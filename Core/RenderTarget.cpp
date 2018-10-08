#include "RenderTarget.h"
#include "KMacro.h"


RenderTarget::RenderTarget()
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