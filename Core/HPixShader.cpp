#include "HPixShader.h"
#include "HVAR.h"
#include "HMACRO.h"


HPixShader::HPixShader()
{
	m_Type = SHTYPE::ST_PS;
}


HPixShader::~HPixShader()
{
	if (nullptr != m_pShader) { m_pShader->Release(); }
}

bool HPixShader::Load(const char* _FuncName, UINT _VH, UINT _VL) 
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "ps_%d_%d", _VH, _VL);

	// 컴파일만 해본거다.
	if (S_OK != D3DCompileFromFile(Path(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		// "strcut {"

		// 이 내부에 있는 데이터를 확인할수 있다.
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		TASSERT(true);
		return false;
	}

	if (S_OK != HVAR::PDevice()->CreatePixelShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pShader))
	{
		return false;
	}

	DefInit();

	return true;
}

void HPixShader::Update() 
{
	HVAR::Context()->PSSetShader(m_pShader, 0, 0);
}

void HPixShader::UpdateCB(HPTR<HShader::CBUFFER> _Buf)
{
	m_pContext->PSSetConstantBuffers(_Buf->iReg, 1, &_Buf->pConstBuffer);
}