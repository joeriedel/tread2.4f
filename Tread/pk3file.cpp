// PK3FILE.CPP
// Copyright © 2000 Joe Riedel and Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "pk3file.h"
#include "unzip.h"
#include "szstring.h"
#include "treadsys.h"
#include "texapi.h"

CPk3File::CPk3File()
{
	m_pUnzFiles = 0;
	m_nNumFiles = 0;
	m_unzFile = 0;
	m_nNumTextures = 0;
	m_pTextures = 0;
	m_pAllTextures = 0;
	m_pShaders = 0;
	m_pTexList = 0;
}

CPk3File::~CPk3File()
{
}

bool CPk3File::ClaimFile(const char* szFileName)
{
	// See if unz can open it.
	unzFile unzFile = unzOpen(szFileName);
	if(unzFile == 0)
		return false;
	unzClose(unzFile);
	return true;
}

void CPk3File::FormatName(CString sName, CString& sFormatted)
{
	// Remove the textures extension.
	sFormatted = sName;

	if(sFormatted.Find("textures/") != -1)
		sFormatted.Delete(0, 9);
	if(sFormatted[sFormatted.GetLength()-4] == '.')
		sFormatted.Delete(sFormatted.GetLength()-4, 4);

}

CQ3Texture* CPk3File::FindTexture(CString sFilename)
{
	int i;
	for(i = 0; i < m_nAllTextures; i++)
	{
		if(m_pAllTextures[i].m_sFilename == sFilename)
			return &m_pAllTextures[i];
	}

	return 0;
}

CQ3Texture* CPk3File::FindTextureInAllPaks(CString sFilename)
{
	int i;
	CQ3Texture* pTex;
	CPakFile* pPak;
	CQ3PakBase* pBase;

	for(i = 0; i < MAX_OPEN_PAKS; i++)
	{
		pPak = GetTexAPI()->GetPakLoader()->GetPakFromIndex(i);
		if(pPak == NULL)
			continue;
		pBase = dynamic_cast<CQ3PakBase*>(pPak);
		if(pBase == NULL)
			continue;
		pTex = pBase->FindTexture(sFilename);
		if(pTex != NULL)
			return pTex;
	}

	return NULL;
}

bool CPk3File::ParseImage(CString sFilename, UNZENTRY* pEntry, int nHandle, CQ3Texture* pTexture)
{
	pTexture->m_bLinkedToShader = false;

	// Better have an extension, and it better be .jpg or .tga.
	if((sFilename.Find(".tga")==-1) && (sFilename.Find(".jpg")==-1))
		return false;

	if(sFilename.Find(".tga") != -1)
		pTexture->m_pReader = &m_TargaReader;
	else
		pTexture->m_pReader = &m_JPGReader;

	pTexture->m_sFilename = sFilename;
	FormatName(sFilename, pTexture->m_sName);
	pTexture->m_sRealName = pTexture->m_sName;
	pTexture->m_nHandle = nHandle;

	return true;
}

int CPk3File::ParseShader(CString sFilename, UNZENTRY* pEntry, int nHandle, QSHADER* pList)
{
	// Load it up.
	char* pText;
	if(!LoadFile(pEntry, (unsigned char**)&pText, pEntry->zInfo.cur_file_info.uncompressed_size))
		return -1;

	int n = CQShaders::ParseShaderScript(pText, pEntry->zInfo.cur_file_info.uncompressed_size, pList);
	delete[] pText;
	return n;
}

bool CPk3File::LoadFile(UNZENTRY* pEntry, unsigned char** pBuffer,  int nLength)
{
	memcpy(pEntry->zFile, &pEntry->zInfo, sizeof(unz_s));
	if(unzOpenCurrentFile(pEntry->zFile) != UNZ_OK)
		return false;
	*pBuffer = new unsigned char[(nLength+4095)/4096*4096];
	if(*pBuffer == NULL)
	{
		unzCloseCurrentFile(pEntry->zFile);
		return false;
	}

	if(unzReadCurrentFile(pEntry->zFile, *pBuffer, (unsigned int)nLength) != nLength)
	{
		delete[] (*pBuffer);
		*pBuffer = 0;
		unzCloseCurrentFile(pEntry->zFile);
		return false;
	}

	unzCloseCurrentFile(pEntry->zFile);
	return true;
}

// Get's the file object.
void* CPk3File::GetFileObject(CTexture* pTex, int nContext)
{
	unsigned char* pData;
	UNZENTRY* pEntry = &m_pUnzFiles[pTex->m_nHandle];

	if(!LoadFile(pEntry, &pData, pEntry->zInfo.cur_file_info.uncompressed_size))
		return 0;

	return pData;
}

void CPk3File::ReleaseFileObject(void* pFile, int nContext)
{
	if(pFile == 0)
		return;
	delete[] (unsigned char*)pFile;
}

int CPk3File::CountShader(CString sFilename, UNZENTRY* pEntry)
{
	// Load it up.
	char* pText;
	if(!LoadFile(pEntry, (unsigned char**)&pText, pEntry->zInfo.cur_file_info.uncompressed_size))
		return -1;

	int n = CQShaders::CountShadersInScript(pText, pEntry->zInfo.cur_file_info.uncompressed_size);
	delete[] pText;
	return n;
}

bool CPk3File::BuildPak()
{
	int i;
	int nFinals, nOff;
	CString sName;
	QSHADER* pShader;
	CQ3Texture* pTex;

	Debug("... %s\n", m_sFilename);
	Debug("... building shader lists/rejecting dumb ones...\n");

	nFinals=0;
	// Link each shader up.
	for(i = 0; i < m_nNumShaders; i++)
	{
		pShader = &m_pShaders[i];
		pShader->pTexture = 0;
		pShader->bIgnored = false;

		if(pShader->sName == "")
		{
			pShader->bIgnored = true;
			continue;
		}

		if(pShader->sTexture != "")
		{
			if(ClassifyFile(pShader->sName) == PK3_IGNORE)
			{
				pShader->bIgnored = true;
				continue;
			}
			pTex = FindTextureInAllPaks(pShader->sTexture);
		}
		else
		{
			sName = pShader->sName+".jpg";
			if(ClassifyFile(sName) == PK3_IGNORE)
			{
				pShader->bIgnored = true;
				continue;
			}
			pTex = FindTextureInAllPaks(sName);
			if(pTex==0)
			{
				sName = pShader->sName+".tga";
				pTex = FindTextureInAllPaks(sName);
			}
		}

		if(pTex == NULL)
			continue;
		pShader->pTexture = pTex;
		pTex->m_bLinkedToShader = true;
		nFinals++;
	}

	Debug("... moving non-linked textures to global scope...\n");

	// Count non-linked textures, they will result in finals as well.
	for(i = 0; i < m_nAllTextures; i++)
	{
		if(m_pAllTextures[i].m_bLinkedToShader == true)
			continue;
		nFinals++;
	}
	
	Debug("... finalizing structs\n");

	if(nFinals == 0)
	{
		// Nothing.
		m_pTextures = 0;
		m_nNumTextures = 0;
		m_pTexList = 0;
		return true;
	}

	// Alloc final texture block.
	m_pTextures = new CQ3Texture[nFinals];
	if(m_pTextures == 0)
		return false;

	m_pTexList = new CTexture*[nFinals];

	m_nNumTextures = nFinals;

	// Load em up.
	nOff=0;
	for(i = 0; i < m_nNumShaders; i++)
	{
		pShader = &m_pShaders[i];
		if(pShader->pTexture == 0)
		{
			if(!pShader->bIgnored)
				Debug("... unable to find texture for shader: %s, %s\n", pShader->sName, pShader->sTexture);
			continue;
		}

		pTex = &m_pTextures[nOff++];
		m_pTexList[nOff-1] = pTex;
		pTex->m_nHandle = pShader->pTexture->m_nHandle;
		pTex->m_sRealName = pTex->m_sFilename = pTex->m_sName = pShader->sName;
		pTex->m_pReader = pShader->pTexture->m_pReader;
		pTex->m_bLinkedToShader = true;
		pTex->m_pShader = pShader;
	}

	for(i = 0; i < m_nAllTextures; i++)
	{
		if(m_pAllTextures[i].m_bLinkedToShader)
			continue;

		pTex = &m_pTextures[nOff++];
		m_pTexList[nOff-1] = pTex;
		pTex->m_nHandle = m_pAllTextures[i].m_nHandle;
		pTex->m_sRealName = m_pAllTextures[i].m_sRealName;
		pTex->m_sFilename = m_pAllTextures[i].m_sFilename;
		pTex->m_sName = m_pAllTextures[i].m_sName;
		pTex->m_pReader = m_pAllTextures[i].m_pReader;
		pTex->m_bLinkedToShader = false;
	}

	Debug("... done\n");

	return true;
}

bool CPk3File::PakBuildsComplete()
{
	// Delete useless textures.
	delete[] m_pAllTextures;
	m_pAllTextures = 0;
	m_nAllTextures = 0;
	return true;
}

// Get's the list of texture's in the pak.
CTexture** CPk3File::GetTextureList(int& nNumTextures) const
{
	nNumTextures = m_nNumTextures;
	return m_pTexList;
}

// Get's all the textures.
CQ3Texture* CPk3File::GetAllTextures(int& nNumTextures) const
{
	nNumTextures = m_nAllTextures;
	return m_pAllTextures;
}

int CPk3File::ClassifyFile(CString sFilename)
{
	// Put some basic directories on the ignore list...
	if(sFilename.Find("models/") != -1 ||
		sFilename.Find("sprites/") != -1 ||
		sFilename.Find("test/") != -1 ||
		sFilename.Find("dont_use/") != -1)
		return PK3_IGNORE;

	if(sFilename.Find(".shader") != -1)
		return PK3_SHADER;
	else
	if((sFilename.Find("textures/") != -1) && ((sFilename.Find(".tga")!=-1) || (sFilename.Find(".jpg")!=-1)))
		return PK3_TEXTURE;

	return PK3_IGNORE;
}

bool CPk3File::OpenAndInitialize(const char* szFileName)
{
	int nType;
	int nNumShaders, nNumTextures, nNumFiles;
	unz_file_info unzFileInfo;
	char szName[1024];
	CString sName;
	UNZENTRY* pUnzFile, UnzTemp;

	m_TargaReader.SetPakFile(this);
	m_JPGReader.SetPakFile(this);

	m_unzFile = unzOpen(szFileName);
	if(m_unzFile == 0)
		return false;
	m_sFilename = szFileName;

	// Count num files.
	nNumShaders = 0;
	nNumTextures = 0;
	UnzTemp.zFile = m_unzFile;
	if(unzGoToFirstFile(m_unzFile) == UNZ_OK)
	{
		for(;;)
		{
			unzGetCurrentFileInfo(m_unzFile, &unzFileInfo, szName, 1024, NULL, 0, NULL, 0);
			StrLower(szName);
			sName = szName;
			sName.Replace('\\', '/');
			nType = ClassifyFile(sName);
			if(nType == PK3_SHADER)
			{
				memcpy(&UnzTemp.zInfo, (unz_s*)m_unzFile, sizeof(unz_s));
				int nCount = CountShader(sName, &UnzTemp);
				if(nCount > 0)
					nNumShaders+=nCount;
			}
			else if(nType == PK3_TEXTURE)
				nNumTextures++;			
			// Next.
			if(unzGoToNextFile(m_unzFile) != UNZ_OK)
				break;
		}
	}
	else
	{
		Debug("pk3: unzGoToFirstFile: failed!\n");
		Close();
		return false;
	}

	m_nNumFiles = nNumTextures+nNumShaders;

	// This time we load and parse.
	m_nAllTextures = nNumTextures;
	m_nNumShaders = nNumShaders;


	if((nNumShaders+nNumTextures)==0)
	{
		Debug("pk3: no useful files found in %s\n", szFileName);
		return true;
	}

	if(m_nNumFiles == 0)
	{
		m_pAllTextures = 0;
		m_pShaders = 0;
		return true;
	}

	if(nNumTextures > 0)
	{
		m_pAllTextures = new CQ3Texture[nNumTextures];
		if(m_pAllTextures == NULL)
		{
			Error("pk3: out of memory!\n");
			Close();			
			return false;
		}
	}
	if(nNumShaders > 0)
	{
		m_pShaders = new QSHADER[nNumShaders];
		if(m_pShaders == NULL)
		{
			Error("pk3: out of memory!\n");
			Close();
			return false;
		}
	}

	m_pUnzFiles = new UNZENTRY[m_nNumFiles];

	nNumShaders = 0;
	nNumTextures = 0;
	nNumFiles = 0;
	if(unzGoToFirstFile(m_unzFile) == UNZ_OK)
	{
		for(;;)
		{
			unzGetCurrentFileInfo(m_unzFile, &unzFileInfo, szName, 1024, NULL, 0, NULL, 0);
			StrLower(szName);
			sName = szName;
			sName.Replace('\\', '/');
			nType = ClassifyFile(sName);
			if(nType != PK3_IGNORE)
			{
				pUnzFile = &m_pUnzFiles[nNumFiles++];
				pUnzFile->nSize = unzFileInfo.uncompressed_size;
				pUnzFile->sName = sName;
				pUnzFile->zFile = m_unzFile;
				memcpy(&pUnzFile->zInfo, (unz_s*)m_unzFile, sizeof(unz_s));

				if(nType == PK3_SHADER)
				{
					Debug("parsing %s\n", sName);
					int nCount = ParseShader(sName, pUnzFile, nNumFiles-1, &m_pShaders[nNumShaders]);
					if(nCount > 0)
						nNumShaders += nCount;
				}
				else if(nType == PK3_TEXTURE)
				{
					if(!ParseImage(sName, pUnzFile, nNumFiles-1, &m_pAllTextures[nNumTextures]))
						continue;
					nNumTextures++;			
				}
			}

			// Next.
			if(unzGoToNextFile(m_unzFile) != UNZ_OK)
				break;
		}
	}
	else
	{
		Debug("pk3: unzGoToFirstFile: failed!\n");
		Close();
		return false;
	}

	return true;
}

bool CPk3File::Close()
{
	// Kill the file.
	if(m_unzFile != 0)
	{
		unzClose(m_unzFile);
		m_unzFile = 0;
	}

	if(m_pAllTextures != 0)
		delete[] m_pAllTextures;
	if(m_pTextures != 0)
		delete[] m_pTextures;
	if(m_pUnzFiles != 0)
		delete[] m_pUnzFiles;
	if(m_pShaders != 0)
		delete[] m_pShaders;
	if(m_pTexList != 0)
		delete[] m_pTexList;

	m_pTexList = 0;
	m_pAllTextures = 0;
	m_pTextures = 0;
	m_pUnzFiles = 0;
	m_pShaders = 0;

	return true;
}