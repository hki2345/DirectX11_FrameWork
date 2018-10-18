#include "HBlend.h"
#include "HVAR.h"
#include "KMacro.h"


HBlend::HBlend() : m_pBlendState(nullptr), m_Color(1, 1, 1, 1)
{
}


HBlend::~HBlend()
{
	if (nullptr != m_pBlendState)
	{
		m_pBlendState->Release();
		m_pBlendState = nullptr;
	}
}

bool HBlend::Create() 
{
	BDesc.AlphaToCoverageEnable = false;
	BDesc.IndependentBlendEnable = false;

	BDesc.RenderTarget[0].BlendEnable = true;
	BDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	BDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	BDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	if (S_OK != HVAR::PDevice()->CreateBlendState(&BDesc, &m_pBlendState))
	{
		KASSERT(true);
		return false;
	}

	return true;
}

bool HBlend::Update() 
{
	HVAR::Context()->OMSetBlendState(m_pBlendState, m_Color.s, 0xffffffff);

	return true;
}

bool HBlend::Create(D3D11_BLEND_DESC _Decs) {
	BDesc = _Decs;
	if (S_OK != HVAR::PDevice()->CreateBlendState(&_Decs, &m_pBlendState))
	{
		KASSERT(true);
		return false;
	}

	return true;
}