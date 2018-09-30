#include "Blend.h"
#include "Core_Class.h"
#include "KMacro.h"


Blend::Blend(): m_BlendState(nullptr), m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
}


Blend::~Blend()
{
	if (nullptr != m_BlendState)
	{
		m_BlendState->Release();
		m_BlendState = nullptr;
	}
}


bool Blend::Create()
{
	m_Desc.AlphaToCoverageEnable = false;
	m_Desc.IndependentBlendEnable = false;

	// ¼Ò½º ÇÈ¼¿ * ÆÑÅÍ + ÀÖ´ø ÇÈ¼¿ * ÆÑÅÍ || ÆÑÅÍ -> ¾î´À Á¤µµ

	m_Desc.RenderTarget[0].BlendEnable = true;
	m_Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	m_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	m_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (S_OK != Core_Class::device()->CreateBlendState(&m_Desc, &m_BlendState))
	{
		KASSERT(true);
		return false;
	}

	return true;
}

void Blend::Update()
{
	Core_Class::device_context()->OMSetBlendState(m_BlendState, m_Color.s, 0xffffffff);
}