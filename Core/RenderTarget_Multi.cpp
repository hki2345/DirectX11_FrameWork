#include "RenderTarget_Multi.h"
#include "Core_Class.h"



RenderTarget_Multi::RenderTarget_Multi() :m_DepthTex(nullptr), m_bDefaultDepth(true)
{
}


RenderTarget_Multi::~RenderTarget_Multi()
{
	if (nullptr != m_DepthStencil)
	{
		m_DepthStencil->Release();
	}
}


void RenderTarget_Multi::SetOM()
{
	if (nullptr != m_DepthTex)
	{
		Core_Class::Context()->OMSetRenderTargets(TargetCount(), &m_RTViewVec[0], m_DepthTex->DepthStencilView());
		Core_Class::Context()->OMSetDepthStencilState(m_DepthStencil, 1);
	}
	else
	{
		// 이 과정ㅇ은 -> 현재 이 멀티 타겟이 뎁스 스텐실을 가진 경우 그대로 하게 되고
		// 아니면 기존 디바이스가 가진 백버퍼의 뎁스스텐실을 적용시킨다는 뜻
		if (false == m_bDefaultDepth)
		{
			ID3D11DepthStencilView* OldDepth;
			Core_Class::Context()->OMGetRenderTargets(0, nullptr, &OldDepth);
			Core_Class::Context()->OMSetRenderTargets(TargetCount(), &m_RTViewVec[0], OldDepth);
			OldDepth->Release();
		}
		else
		{
			Core_Class::Context()->OMSetRenderTargets(TargetCount(), &m_RTViewVec[0],
				Core_Class::MainDevice().DepthStencil_View());
		}
	}
}

void RenderTarget_Multi::Clear()
{
	for (size_t i = 0; i < m_RTVec.size(); i++)
	{
		m_RTVec[i]->Clear();
	}

	if (nullptr != m_DepthTex)
	{
		Core_Class::Context()->ClearDepthStencilView(m_DepthTex->DepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
}



void RenderTarget_Multi::Create_Target(const KUINT& _W, const KUINT& _H, const KUINT& _BindFlag,
	DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage/* = D3D11_USAGE_DEFAULT*/)
{
	KPtr<RenderTarget> NewT = new RenderTarget();
	if (false == NewT->Create(_W, _H, _BindFlag, _eFormat, _eUsage))
	{
		KASSERT(true);
	}

	m_RTViewVec.push_back(NewT->texture()->Render_TargetView());
	m_RTVec.push_back(NewT);
}

// 이 Desc는 렌더 타겟마다 달라질 수 있다.
void RenderTarget_Multi::Create_Depth(const KUINT& _W, const KUINT& _H)
{
	m_DepthTex = new Texture();
	if (false == m_DepthTex->Create(_W, _H, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D24_UNORM_S8_UINT))
	{
		KASSERT(true);
		return;
	}

	m_DepthDesc.DepthEnable = TRUE;
	m_DepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_DepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	m_DepthDesc.StencilEnable = FALSE;
	m_DepthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_DepthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC defaultStencilOp =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	m_DepthDesc.FrontFace = defaultStencilOp;
	m_DepthDesc.BackFace = defaultStencilOp;


	Core_Class::Device()->CreateDepthStencilState(&m_DepthDesc, &m_DepthStencil);

	if (nullptr == m_DepthStencil)
	{
		KASSERT(true);
		return;
	}

	return;

}


KPtr<Texture> RenderTarget_Multi::texture(const KUINT& _Idx)
{
	return m_RTVec[_Idx]->texture();
}
