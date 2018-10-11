#pragma once
#include "RenderTarget.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "KMacro.h"


// ���ҽ� ����Ʈ ����
// define���� ���� �Ǿ� �ְ� c++ �̳� ����� ��Ʃ����� ����
// �޶�����.
// https://docs.microsoft.com/en-us/windows/desktop/direct3d11/overviews-direct3d-11-resources-limits
// �ش� �ּҴ� ���̴��� �� ���� ���� �� �ִ� ���� �� -> ���� ����� ��������

class RenderTarget_Multi : public KResource
{
public:
	RenderTarget_Multi();
	~RenderTarget_Multi();


private:
	friend RenderManager;

private:
	std::vector<KPtr<RenderTarget>> m_RTVec;
	std::vector<ID3D11RenderTargetView*> m_RTViewVec;

	KPtr<Texture> m_DepthTex;
	D3D11_DEPTH_STENCIL_DESC m_DepthDesc;
	ID3D11DepthStencilState* m_DepthStencil;

public:
	KUINT TargetCount()
	{
		return (KUINT)m_RTVec.size();
	}

public:
	void SetOM();
	void Clear();
	void Create_Depth(const KUINT& _W, const KUINT& _H);


public:
	template<typename... Rest>
	bool Create(Rest... _Arg)
	{
		m_RTVec.reserve(sizeof...(_Arg));
		Insert_Target(_Arg...);
		return true;
	}

private:
	template<typename... Rest>
	void Insert_Target(const wchar_t* _TargetName, Rest... _Arg)
	{
		KPtr<RenderTarget> Target = ResourceManager<RenderTarget>::Find(_TargetName);

		KASSERT(nullptr == Target);

		m_RTVec.push_back(Target);
		m_RTViewVec.push_back(Target->texture()->Render_TargetView());
		Insert_Target(_Arg...);
	}

	void Insert_Target() {		}

public:
	std::vector<KPtr<RenderTarget>> RenderTargetList()
	{
		return m_RTVec;
	}

};

