#include "stdafx.h"
#include "AccelFile.h"
#include "TreadSys.h"

//default constructor
CAccelFile::CAccelFile()
{
	m_nVersion = ACCEL_VER;
}

//default destructor
CAccelFile::~CAccelFile()
{

}

//Open an accelerator table file for reading or writing
int CAccelFile::Open(CString sFilePath, UINT nMode)
{
	if(sFilePath.IsEmpty())
		return 0;

	UINT nParam = 0;

	if(nMode == READ)
		nParam = CFile::modeRead
				| CFile::modeNoTruncate
				| CFile::shareDenyWrite
				| CFile::typeBinary;
	else
		nParam = CFile::modeWrite
				| CFile::modeCreate
				| CFile::shareDenyRead
				| CFile::typeBinary;

	return m_hFile.Open(sFilePath, nParam, NULL);
}

//Abort the file and ignore any error information
void CAccelFile::Abort()
{
	m_hFile.Abort();
}

//Close an accelerator table file
void CAccelFile::Close()
{
	m_hFile.Close();
}

//Write a table to the file
void CAccelFile::WriteTable(ACCEL* pTable, int nMax)
{
	m_hFile.Write(ACCEL_FILE, 4);
	m_hFile.Write(&m_nVersion, 4);
	

	if((pTable == NULL) || (nMax <= 0))
	{
		m_hFile.Write(0, 4);
		return;
	}
	else
		m_hFile.Write(&nMax, 4);

	for(int n = 0; n < nMax; n++)
	{
		m_hFile.Write(&pTable[n].fVirt, 1);
		m_hFile.Write(&pTable[n].key, 2);
		m_hFile.Write(&pTable[n].cmd, 2);
	}
}

//Read a table from the file and store in reference variable
int CAccelFile::ReadTable(ACCEL* pTable, int nMax)
{
	if((pTable == NULL)  || (nMax <= 0))
		return -1;

	int nTemp;

	m_hFile.Read(&nTemp, 4);	//Id
	if(nTemp != 1279607617)
		return 0;
	m_hFile.Read(&nTemp, 4);	//Version
	m_hFile.Read(&nTemp, 4);	//Item count

	for(int n = 0; (n < nTemp) || (n < nMax); n++)
	{
		m_hFile.Read(&pTable[n].fVirt, 1);
		m_hFile.Read(&pTable[n].key, 2);
		m_hFile.Read(&pTable[n].cmd, 2);
	}

	return nTemp;
}

//Read an accelerator table from the file into an ACCEL array
ACCEL* CAccelFile::ReadTable(int &nCount)
{
	int nTemp;
	ACCEL* pTable = NULL;

	m_hFile.Read(&nTemp, 4);	//ID
	if(nTemp != 1279607617)
	{
		nCount = 0;
		return NULL;
	}
	m_hFile.Read(&nTemp, 4);	//Version
	m_hFile.Read(&nCount, 4);	//Item count

	if(nCount <= 0)
		return NULL;

	pTable = new ACCEL[nCount];
	if(pTable == NULL)
		return NULL;

	for(int n = 0; n < nCount; n++)
	{
		m_hFile.Read(&pTable[n].fVirt, 1);
		m_hFile.Read(&pTable[n].key, 2);
		m_hFile.Read(&pTable[n].cmd, 2);
	}

	return pTable;
}