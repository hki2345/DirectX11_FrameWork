#include "HShader.h"
#include "Stl_AID.h"
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

KPtr<HShader::CBUFFER> HShader::FindCB(const wchar_t* _Name)
{
	return Map_Find<KPtr<HShader::CBUFFER>>(m_MapConstBuffer, _Name);
}
