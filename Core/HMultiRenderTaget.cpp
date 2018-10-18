#include "HMultiRenderTaget.h"
#include "HVAR.h"


HMultiRenderTaget::HMultiRenderTaget() : m_DepthTex(nullptr), m_bDefaultDepth(true)
{
}


HMultiRenderTaget::~HMultiRenderTaget()
{
	if (nullptr != m_pDepthStencilState)
	{
		m_pDepthStencilState->Release();
	}
}

void HMultiRenderTaget::CreateDepth(unsigned int _W, unsigned int _H)
{
	m_DepthTex = new HTexture();
	if (false == m_DepthTex->Create(_W, _H, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D24_UNORM_S8_UINT))
	{
		BBY;
		return;
	}

	m_DepthState.DepthEnable = TRUE;
	m_DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	m_DepthState.StencilEnable = FALSE;
	m_DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC defaultStencilOp =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	m_DepthState.FrontFace = defaultStencilOp;
	m_DepthState.BackFace = defaultStencilOp;


	HVAR::PDevice()->CreateDepthStencilState(&m_DepthState, &m_pDepthStencilState);

	if (nullptr == m_pDepthStencilState)
	{
		KASSERT(true);
		return;
	}

	return;
}

void HMultiRenderTaget::OMSet() 
{
	if (nullptr != m_DepthTex)
	{
		HVAR::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], m_DepthTex->DSV());
		HVAR::Context()->OMSetDepthStencilState(m_pDepthStencilState, 1);
	} else 
	{
		if (false == m_bDefaultDepth)
		{
			ID3D11DepthStencilView* OldDepth;
			HVAR::Context()->OMGetRenderTargets(0, nullptr, &OldDepth);
			HVAR::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], OldDepth);
			// HVAR::Context()->OMSetDepthStencilState(m_pDepthStencilState, 1);
			OldDepth->Release();
		}
		else {
			HVAR::Context()->OMSetRenderTargets(TagetCount(), &m_RenderTagetView[0], HVAR::MainDevice().Depth());
		}

	}
}

void HMultiRenderTaget::Clear() 
{
	for (size_t i = 0; i < m_RenderTaget.size(); i++)
	{
		m_RenderTaget[i]->Clear();
	}

	if (nullptr != m_DepthTex)
	{
		HVAR::Context()->ClearDepthStencilView(m_DepthTex->DSV()
			, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
}

void HMultiRenderTaget::CreateTaget(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage )
{
	KPtr<HRenderTaget> Taget = new HRenderTaget();
	if (false == Taget->Create(_W, _H, _BindFlag, _eFormat, _eUsage))
	{
		BBY;
	}

	m_RenderTagetView.push_back(Taget->TagetTex()->RTV());
	m_RenderTaget.push_back(Taget);
}

KPtr<HTexture> HMultiRenderTaget::TagetTex(unsigned int _Idx)
{
	return m_RenderTaget[_Idx]->TagetTex();
}