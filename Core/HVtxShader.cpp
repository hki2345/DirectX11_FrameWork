#include "HVtxShader.h"
#include "HVAR.h"
#include "HMACRO.h"	


HVtxShader::HVtxShader() : m_Offset(0)
{
	m_Type = SHTYPE::ST_VS;
}


HVtxShader::~HVtxShader()
{
	if (nullptr != m_pShader) { m_pShader->Release(); }
	if (nullptr != m_pInputLayout) { m_pInputLayout->Release(); }
}

bool HVtxShader::Load(const char* _FuncName, UINT _VH, UINT _VL) 
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "vs_%d_%d", _VH, _VL);

	// _FuncName == VS_Color

	// 컴파일만 해본거다.
	if (S_OK != D3DCompileFromFile(Path(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		TASSERT(true);

		return false;
	}

	if (S_OK != HVAR::PDevice()->CreateVertexShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pShader))
	{
		return false;
	}

	DefInit();

	return true;
}

void HVtxShader::Update() {
	HVAR::Context()->VSSetShader(m_pShader, 0, 0);
}

bool HVtxShader::AddLayoutEnd(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC /*= D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA*/, UINT _IDSR /*= 0*/)
{
	AddLayout(_SName, _SI, _Fm, _IS, _ISC, _IDSR);
	CreateLayout();

	return true;
}

bool HVtxShader::AddLayout(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm
	, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC, UINT _IDSR) 
{
	D3D11_INPUT_ELEMENT_DESC _NewInputLayout = D3D11_INPUT_ELEMENT_DESC();

	if (_IS >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT)
	{
		return false;
	}

	_NewInputLayout.SemanticName = _SName;
	_NewInputLayout.SemanticIndex = _SI;
	_NewInputLayout.Format = _Fm;
	_NewInputLayout.InputSlot  = _IS;
	_NewInputLayout.InputSlotClass = _ISC;
	_NewInputLayout.InstanceDataStepRate = _IDSR;
	_NewInputLayout.AlignedByteOffset = m_Offset;

	m_Offset += DHelper::GetFmSize(_Fm);
	m_InputLayoutData.push_back(_NewInputLayout);

	return true;
}

bool HVtxShader::CreateLayout() 
{
	TASSERT(nullptr == m_pShader);
	if (nullptr == m_pShader)
	{
		return false;
	}

	if (S_OK != HVAR::PDevice()->CreateInputLayout(&m_InputLayoutData[0], (UINT)m_InputLayoutData.size(), m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), &m_pInputLayout) )
	{
		return false;
	}

	return true;
}

void HVtxShader::SetLayout() 
{
	HVAR::Context()->IASetInputLayout(m_pInputLayout);
}

void HVtxShader::UpdateCB(HPTR<HVtxShader::CBUFFER> _Buf) 
{
	m_pContext->VSSetConstantBuffers(_Buf->iReg, 1, &_Buf->pConstBuffer);
}