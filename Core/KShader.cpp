#include "KShader.h"
#include "Stl_Assistor.h"
#include "Core_Class.h"


KShader::KShader() : m_Type(SHADER_TYPE::ST_NONE)
{
}


KShader::~KShader()
{
	// �� �������� �Լ��� ���� �̰͵��� ����Ʈ �����ͷ� �����ִٴ� ���� �� �� �ִ�.
	// ���� ���⼭ ����Ʈ ���� ������ ���� ��
	if (nullptr != m_Blob) { m_Blob->Release(); }
	if (nullptr != m_Blob_Error) { m_Blob_Error->Release(); }
}




void KShader::Init_Default()
{
	m_pContext = Core_Class::Context();
}
bool KShader::Create_ConstBuf(Const_Buffer* _NewBuf)
{
	if (S_OK != Core_Class::Device()->CreateBuffer(&_NewBuf->tDesc, nullptr, &_NewBuf->p_ConstBuffer))
	{
		delete _NewBuf;
		return false;
	}
	return true;
}

KPtr<KShader::Const_Buffer> KShader::Find_ConstBuffer(const wchar_t* _Name)
{
	return Map_Find<KPtr<KShader::Const_Buffer>>(m_CB_Map, _Name);
}