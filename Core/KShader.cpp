#include "KShader.h"
#include "Stl_Assistor.h"
#include "Core_Class.h"


KShader::KShader() : m_Type(SHADER_TYPE::ST_NONE)
{
}


KShader::~KShader()
{
	// 이 포인터의 함수를 보면 이것들이 스마트 포인터로 엮여있다는 것을 알 수 있다.
	// 물론 여기서 스마트 폰은 지들이 만든 거
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