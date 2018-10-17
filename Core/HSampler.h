#pragma once
#include "HRes.h"
#include "DHeader.h"

class HSampler : public HRes
{
private:
	D3D11_SAMPLER_DESC m_Desc;
	ID3D11SamplerState* m_pState;

public:
	ID3D11SamplerState* PState() 
	{
		return m_pState;
	}

	// 일단 그냥 디폴트 세팅으로 만든다.
	bool Create();

	bool Setting(D3D11_FILTER Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
	FLOAT MipLODBias = 1.0f,
	UINT MaxAnisotropy = 0,
	D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER,
	HCOLOR BorderColor = HVEC::ONE,
	FLOAT MinLOD = -FLT_MAX,
	FLOAT MaxLOD = FLT_MAX
	);

	//bool Setting(D3D11_FILTER Filter = D3D11_FILTER_ANISOTROPIC,
	//	D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
	//	D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
	//	D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
	//	FLOAT MipLODBias = 1.0f,
	//	UINT MaxAnisotropy = 0,
	//	D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER,
	//	HVEC BorderColor = HVEC::ONE,
	//	FLOAT MinLOD = -FLT_MAX,
	//	FLOAT MaxLOD = FLT_MAX
	//);

public:
	void Update(unsigned int _Slot);

public:
	HSampler();
	~HSampler();
};

