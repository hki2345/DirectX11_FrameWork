#include "RenderTarget.h"
#include "KMacro.h"
#include "Core_Class.h"

RenderTarget::RenderTarget() :m_Color(KColor::Black)
{
}


RenderTarget::~RenderTarget()
{
}


bool RenderTarget::Create(const unsigned int& _W, const unsigned int& _H, const unsigned int& _BindFlag,
	DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage/* = D3D11_USAGE::D3D11_USAGE_DEFAULT*/)
{
	m_Texture = new Texture();
	if (false == m_Texture->Create(_W, _H, _BindFlag, _eFormat, _eUsage))
	{
		m_Texture = nullptr;
		KASSERT(true);
	}

	return true;
}

bool RenderTarget::Create(ID3D11Texture2D* _pTex2D, const unsigned int& _BindFlag)
{
	m_Texture = new Texture();
	if (false == m_Texture->Create(_pTex2D, _BindFlag))
	{
		m_Texture = nullptr;
		KASSERT(true);
	}

	return true;
}

void RenderTarget::Clear()
{
	if (nullptr == m_Texture|| nullptr == m_Texture->Render_TargetView())
	{
		KASSERT(true);
	}

	// Color s -> 포인터 형으로 넘김 -> 컬러를 ㅇㅇ
	Core_Class::Context()->ClearRenderTargetView(m_Texture->Render_TargetView(), m_Color.s);
}
