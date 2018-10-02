#pragma once
#include "KResource.h"
#include "DirectXHeader.h"


class Sampler : public KResource
{
private:
	D3D11_SAMPLER_DESC	m_Desc;
	ID3D11SamplerState* m_State;

public:
	ID3D11SamplerState* state() { return m_State; }

	bool Create();
	bool Set_State(
		D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_POINT,
		// D3D11_FILTER Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		// D3D11_FILTER Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
		FLOAT MipLODBias = 1.0f,
		UINT MaxAnisotropy = 0,
		D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER,
		KColor BorderColor = KVector4(.0f, .0f, 1.0f) /* 요부분도ㅓ */,
		FLOAT MinLOD = 0 /* -FLT_MAX */,
		FLOAT MaxLOD = 0 /* FLT_MAX */
		// 주석 부분은 한번 해보라능 
	);

public:
	void Update(const UINT& _Slot);

public:
	Sampler();
	~Sampler();
};

