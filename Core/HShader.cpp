#include "HShader.h"
#include "StlHelperFunc.h"
#include "HVAR.h"

HShader::HShader() : m_Type(ST_NONE)
{
}

HShader::~HShader()
{
	if (nullptr != m_pBlob)	{		m_pBlob->Release();	}
	if (nullptr != m_pErrBlob)	{		m_pErrBlob->Release();	}
}

void HShader::DefInit() 
{
	m_pContext = HVAR::Context();
}

bool HShader::CreateCB(CBUFFER* NewBuf)
{
	if (S_OK != HVAR::PDevice()->CreateBuffer(&NewBuf->tDesc, nullptr, &NewBuf->pConstBuffer))
	{
		delete NewBuf;
		return false;
	}

	return true;
}

HPTR<HShader::CBUFFER> HShader::FindCB(const wchar_t* _Name)
{
	return MapFind<HPTR<HShader::CBUFFER>>(m_MapConstBuffer, _Name);
}
