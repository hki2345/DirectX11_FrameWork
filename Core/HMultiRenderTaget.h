#pragma once
#include "HRenderTaget.h"
#include "HRenderMgr.h"
#include "HResMgr.h"
#include "KMacro.h"

// 리소스 리미트 개수
// define으로 정의 되어 있고 c++ 이나 비쥬얼 스튜디오에 따라서
// 달라진다.
// https://docs.microsoft.com/en-us/windows/desktop/direct3d11/overviews-direct3d-11-resources-limits

class HMultiRenderTaget : public Resource
{
public:
	friend HRenderMgr;

private:
	std::vector<KPtr<HRenderTaget>> m_RenderTaget;
	std::vector<ID3D11RenderTargetView*> m_RenderTagetView;

	// Depth
	bool m_bDefaultDepth;
	KPtr<HTexture> m_DepthTex;
	D3D11_DEPTH_STENCIL_DESC	m_DepthState;
	ID3D11DepthStencilState*	m_pDepthStencilState;

public:
	// UINT 윈도우에 정의도어 있어서 WINDOW
	unsigned int TagetCount() 
	{
		return (int)m_RenderTaget.size();
	}

public:
	void OMSet();
	void Clear();
	void CreateDepth(unsigned int _W, unsigned int _H);


public:
	template<typename... Rest>
	bool Create(Rest... _Arg)
	{
		m_RenderTaget.reserve(sizeof...(_Arg));
		InsertTaget(_Arg...);
		return true;
	}

private:
	template<typename... Rest>
	void InsertTaget(const wchar_t* _TagetName, Rest... _Arg)
	{
		KPtr<HRenderTaget> Taget = ResourceManager<HRenderTaget>::Find(_TagetName);

		if (nullptr == Taget)
		{
			BBY;
			return;
		}

		m_RenderTaget.push_back(Taget);
		m_RenderTagetView.push_back(Taget->TagetTex()->RTV());
		InsertTaget(_Arg...);
	}

	void InsertTaget() {		}

public:
	void CreateTaget(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

	std::vector<KPtr<HRenderTaget>> TagetTexList() 
	{
		return m_RenderTaget;
	}

	KPtr<HTexture> TagetTex(unsigned int _Idx);

public:
	HMultiRenderTaget();
	~HMultiRenderTaget();
};

