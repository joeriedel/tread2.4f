// QSHADERS.CPP
// Copyright © 2000 Joe Riedel, Nick Randal.
// Author: Joe Riedel.
#include "stdafx.h"
#include "qshaders.h"
#include "tokenizer.h"
#include "treadsys.h"

bool CQShaders::SkipShader(CTokenizer& t, int nBrace)
{
	CString sTk;

	for(;;)
	{
		if(!t.GetToken(sTk))
		{
			Debug("premature end of script\n");
			return false;
		}

		if(sTk == "{")
			nBrace++;
		else if(sTk == "}")
		{
			nBrace--;
			if(nBrace<=0)
			{
				if(nBrace<0)
				{
					Debug("mismatched brace!\n");
					return false;
				}
				break;
			}
		}
	}
	
	return true;
}

int CQShaders::CountShadersInScript(const char* szShaderScript, int nLen)
{
	int nBrace;
	CString sTk;
	CTokenizer t;
	int nNumShaders;

	// We scan for the braces.
	if(!t.InitParsing(szShaderScript, nLen))
		return 0;

	nBrace=0;
	nNumShaders = 0;
	for(;;)
	{
		if(!t.GetToken(sTk))
			break;

		if(sTk == "{")
			nBrace++;
		else if(sTk == "}")
		{
			nBrace--;
			if(nBrace <= 0)
			{
				if(nBrace < 0)
				{
					Debug("Warning: ShaderParse error: mismatched brace!\n");
					return 0;
				}
				nNumShaders++;
			}
		}
	}

	if(nBrace != 0)
	{
		Debug("Warning: ShaderParse error: unexpected eof!\n");
		return 0;
	}

	return nNumShaders;
}

int CQShaders::ParseShaderScript(const char* szShaderScript, int nLen, QSHADER* pList)
{
	CTokenizer t;
	CString sTk;
	int nNum;
	int nBrace;
	QSHADER* pShader;

	if(!t.InitParsing(szShaderScript, nLen))
		return -1;

	nNum=0;
	for(;;)
	{
		nBrace=0;
		if(!t.GetToken(sTk))
			break;
		pShader = &pList[nNum];
		pShader->sName = sTk;

		//Debug("... %s\n", sTk);

		// {
		if(!t.GetToken(sTk))
		{
			Debug("premature end of script!\n");
			return -1;
		}

		if(sTk != "{")
		{
			Debug("expected '{'\n");
			return -1;
		}
		
		nBrace++;

		pShader->sTexture = "";
		for(;;)
		{
			if(!t.GetToken(sTk))
			{
				Debug("premature end of script!\n");
				return -1;
			}

			if(sTk == "{")
			{
				nBrace++;
				continue;
			}
			else if(sTk == "}")
			{
				nBrace--;
				if(nBrace <= 0)
				{
					if(nBrace < 0)
					{
						Debug("mismatched brace!\n");
						return -1;
					}

					break;;
				}
				continue;
			}

			if(sTk == "qer_editorimage")
			{
				// Best case.
				if(!t.GetToken(sTk))
				{
					Debug("premature end of script\n");
					return -1;
				}

				pShader->sTexture = sTk;
				SkipShader(t, nBrace);
				break;
			}
			else if(sTk == "map")
			{
				if(!t.GetToken(sTk))
				{
					Debug("premature end of script!\n");
					return -1;
				}

				// Take only first 'map' token.
				if(pShader->sTexture != "" || sTk == "$lightmap")
					continue;

				pShader->sTexture = sTk;
			}
		}

		nNum++;
	}

	Debug("... parsed %d shader(s)\n", nNum);
	return nNum;
}
