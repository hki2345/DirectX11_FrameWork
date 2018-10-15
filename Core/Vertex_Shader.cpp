#include "Vertex_Shader.h"
#include "Core_Class.h"
#include "KMacro.h"


Vertex_Shader::Vertex_Shader() : m_Offset(0)
{
	m_Type = SHADER_TYPE::ST_VS;
}


Vertex_Shader::~Vertex_Shader()
{
	if (nullptr != m_Shader) { m_Shader->Release(); }
	if (nullptr != m_InputLayout) { m_InputLayout->Release(); }
}



bool Vertex_Shader::Load(const char* _Func, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

	// 11이든 10이든 짜피 같은 세이더 디버그로 된다. -> 애초에 11은 있지도 않음
#ifdef  _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif //  _DEBUG

	char tBuffer[256] = {};
	sprintf_s(tBuffer, "vs_%d_%d", _VH, _VL);

	std::wstring ss = path();

	// 컴파일 만 해보는 것이다 ㅇㅇㅇㅇㅇㅇ
	if (S_OK != D3DCompileFromFile(path(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_Func, tBuffer, iFlag, 0, &m_Blob, &m_Blob_Error))
	{
		std::string ErrorText = (char*)m_Blob_Error->GetBufferPointer();
		KASSERT(true);

		return false;
	}


	// 해당 디바이스에 버텍스세이더를 세팅
	if (S_OK != Core_Class::Device()->CreateVertexShader(
		m_Blob->GetBufferPointer(),
		m_Blob->GetBufferSize(),
		nullptr, &m_Shader))
	{
		return false;
	}

	KShader::Init_Default();

	return true;
}

void Vertex_Shader::Update()
{
	// 여기서는 레이아웃을 돌린다.
	Core_Class::Context()->VSSetShader(m_Shader, 0, 0);
}


bool Vertex_Shader::Add_Layout(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS, 
	D3D11_INPUT_CLASSIFICATION _ISC, UINT _IDSR)
{
	D3D11_INPUT_ELEMENT_DESC NewLayout = D3D11_INPUT_ELEMENT_DESC();

	if (_IS >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT)
	{
		return false;
	}

	NewLayout.SemanticName = _SName;
	NewLayout.SemanticIndex = _SI;
	NewLayout.Format = _Fm;
	NewLayout.InputSlot = _IS;
	NewLayout.InputSlotClass = _ISC;
	NewLayout.InstanceDataStepRate = _IDSR;
	NewLayout.AlignedByteOffset = m_Offset;

	m_Offset += VertShaderFormat::size_Formet(_Fm);
	m_LayOutData_Vec.push_back(NewLayout);
	return true;
}


bool Vertex_Shader::Add_LayoutFin(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS,
	D3D11_INPUT_CLASSIFICATION _ISC, UINT _IDSR)
{
	Add_Layout(_SName, _SI, _Fm, _IS, _ISC, _IDSR);
	Create_LayOut();
	return true;
}


bool Vertex_Shader::Create_LayOut()
{
	KASSERT(nullptr == m_Shader);
	if (nullptr == m_Shader)
	{
		return false;
	}

	if (S_OK != Core_Class::Device()->CreateInputLayout(
		&m_LayOutData_Vec[0], (UINT)m_LayOutData_Vec.size(),
		m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), &m_InputLayout))
	{
		return false;
	}

	return true;
}

void Vertex_Shader::Set_LayOut()
{
	Core_Class::Context()->IASetInputLayout(m_InputLayout);
}


void Vertex_Shader::UpdateCB(KPtr<Const_Buffer> _Buf)
{
	m_pContext->VSSetConstantBuffers(_Buf->iReg, 1, &_Buf->p_ConstBuffer);
}