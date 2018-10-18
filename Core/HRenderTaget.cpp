#include "HRenderTaget.h"
#include "KMacro.h"
#include "HVAR.h"

HRenderTaget::HRenderTaget() : m_Color(HVEC::BLUE)
{
}

HRenderTaget::~HRenderTaget()
{
}

bool HRenderTaget::Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage) 
{
	m_Tex = new HTexture();
	if (false == m_Tex->Create(_W, _H, _BindFlag, _eFormat, _eUsage))
	{
		m_Tex = nullptr;
		BBY;
	}
	return true;
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

	HVAR::Context()->ClearRenderTargetView(m_Tex->RTV(), m_Color.s);
	
}