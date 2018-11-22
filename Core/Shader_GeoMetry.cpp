#include "Shader_GeoMetry.h"
#include "Core_Class.h"
#include "KMacro.h"


Shader_GeoMetry::Shader_GeoMetry()
{
	m_Type = SHTYPE::ST_DOM;
}


Shader_GeoMetry::~Shader_GeoMetry()
{
	if (nullptr != m_pShader) { m_pShader->Release(); }
}

bool Shader_GeoMetry::Load(const char* _FuncName, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

	// 11�̵� 10�̵� ¥�� ���� ���̴� ����׷� �ȴ�. -> ���ʿ� 11�� ������ ����
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "gs_%d_%d", _VH, _VL);

	// ������ �� �غ��� ���̴� ������
	if (S_OK != D3DCompileFromFile(AllPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrBlob))
	{
		// "strcut {"

		// �� ���ο� �ִ� �����͸� Ȯ���Ҽ� �ִ�.
		std::string ErrorText = (char*)m_pErrBlob->GetBufferPointer();
		KASSERT(true);
		return false;
	}

	// �ش� ����̽��� �ȼ����̴��� ����
	if (S_OK != Core_Class::PDevice()->CreateGeometryShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr, &m_pShader))
	{
		return false;
	}

	Init_Default();

	return true;
}

void Shader_GeoMetry::Update()
{
	// ���õ� �����͸� ������ ��
	Core_Class::Context()->GSSetShader(m_pShader, 0, 0);
}

void Shader_GeoMetry::UpdateCB(KPtr<KShader::ConstBuffer> _Buf)
{
	m_pContext->GSSetConstantBuffers(_Buf->iReg, 1, &_Buf->pConstBuffer);
}