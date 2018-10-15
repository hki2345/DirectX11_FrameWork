#include "Pixel_Shader.h"
#include "Core_Class.h"


Pixel_Shader::Pixel_Shader()
{
	m_Type = SHADER_TYPE::ST_PS;
}


Pixel_Shader::~Pixel_Shader()
{
	if (nullptr != m_Shader) { m_Shader->Release(); }
}

bool Pixel_Shader::Load(const char* _Func, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

	// 11�̵� 10�̵� ¥�� ���� ���̴� ����׷� �ȴ�. -> ���ʿ� 11�� ������ ����
#ifdef  _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif //  _DEBUG

	char tBuffer[256] = {};
	sprintf_s(tBuffer, "ps_%d_%d", _VH, _VL);

	// ������ �� �غ��� ���̴� ������
	if (S_OK != D3DCompileFromFile(path(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_Func, tBuffer, iFlag, 0, &m_Blob, &m_Blob_Error))
	{
		std::string ErrorText = (char*)m_Blob_Error->GetBufferPointer();
		KASSERT(true);

	}


	// �ش� ����̽��� �ȼ����̴��� ����
	if (S_OK != Core_Class::Device()->CreatePixelShader(
		m_Blob->GetBufferPointer(),
		m_Blob->GetBufferSize(),
		nullptr, &m_Shader))
	{
		return false;
	}

	KShader::Init_Default();

	return true;
}

void Pixel_Shader::Update()
{
	// ���õ� �����͸� ������ ��
	Core_Class::Context()->PSSetShader(m_Shader, 0, 0);
}

void Pixel_Shader::UpdateCB(KPtr<Const_Buffer> _Buf)
{
	m_pContext->PSSetConstantBuffers(_Buf->iReg, 1, &_Buf->p_ConstBuffer);
}