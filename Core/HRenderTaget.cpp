#include "HRenderTaget.h"
#include "KMacro.h"
#include "Core_Class.h"

HRenderTaget::HRenderTaget() : m_Color(HVEC::BLUE)
{
}

HRenderTaget::~HRenderTaget()
{
}

bool HRenderTaget::Create(const KUINT& _W, const KUINT& _H, HVEC _Color, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/)
{
	m_Tex = new HTexture();
	if (false == m_Tex->Create(_W, _H, _BindFlag, _eFormat, _eUsage))
	{
		m_Tex = nullptr;
		BBY;
	}
	m_Color = _Color;

	return true;
}

bool HRenderTaget::Create(const KUINT& _W, const KUINT& _H, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage)
{
	return Create(_W, _H, HVEC::BLUE, _BindFlag, _eFormat, _eUsage);
}

bool HRenderTaget::Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag) 
{
	m_Tex = new HTexture();
	if (false == m_Tex->Create(_pTex2D, _BindFlag))
	{
		m_Tex = nullptr;
		BBY;
	}
	return true;
}

void HRenderTaget::Clear() 
{
	if (nullptr == m_Tex || nullptr == m_Tex->RTV())
	{
		BBY;
	}

	Core_Class::Context()->ClearRenderTargetView(m_Tex->RTV(), m_Color.s);
	
}