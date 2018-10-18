#pragma once
#include "HTexture.h"
#include "KMacro.h"

// 랜더링의 속력은 무엇으로 결정되는가?

// 첫번째 출력 타겟횟수
// 두번째 출력 랜더횟수
// 세번째 출력 내부연산 

// 장점
// 1. 여러가지 필요한 값들을 모아놔서 그 타겟이 다른곳에 이용될수 있다.
// 단점
// 1. 기본적으로 연산이 늘기 때문에. 1~2개만 출력하려면 
// 2. 알파처리가 힘들다. 나중에 따로 처리 해줘야 한다.

// 빛연산에 있어서 디퍼드 랜더라는것은 타겟횟수를 늘리고
// 내부연산을 극적으로 줄이는것.

// 멀티랜더 타겟이.
class HRenderTaget : public HRes
{
private:
	KPtr<HTexture> m_Tex;
	HVEC		   m_Color;

public:
	KPtr<HTexture> TagetTex() 
	{
		return m_Tex;
	}

public:
	bool Create(const KUINT& _W, const KUINT& _H, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(const KUINT& _W, const KUINT& _H, HVEC _Color, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag);

	void Clear();

public:
	HRenderTaget();
	~HRenderTaget();
};

