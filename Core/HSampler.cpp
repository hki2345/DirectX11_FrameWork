#include "HSampler.h"
#include "Core_Class.h"


HSampler::HSampler()
{
}


HSampler::~HSampler()
{
	if (nullptr != m_pState)
	{
		m_pState->Release();
	}
}


bool HSampler::Create() 
{
	if (false == Setting())
	{
		return false;
	}

	return true;
}


bool HSampler::Setting(D3D11_FILTER Filter,
	D3D11_TEXTURE_ADDRESS_MODE AddressU,
	D3D11_TEXTURE_ADDRESS_MODE AddressV,
	D3D11_TEXTURE_ADDRESS_MODE AddressW,
	FLOAT MipLODBias,
	UINT MaxAnisotropy,
	D3D11_COMPARISON_FUNC ComparisonFunc,
	KColor BorderColor,
	FLOAT MinLOD,
	FLOAT MaxLOD) 
{
	if (nullptr != m_pState)
	{
		m_pState->Release();
	}

	m_Desc.Filter = Filter;
	m_Desc.AddressU = AddressU;
	m_Desc.AddressV = AddressV;
	m_Desc.AddressW = AddressW;
	memcpy_s(m_Desc.BorderColor, sizeof(KColor), BorderColor.s, sizeof(KColor));
	m_Desc.ComparisonFunc = ComparisonFunc;
	m_Desc.MaxAnisotropy = MaxAnisotropy;
	m_Desc.MipLODBias = MipLODBias;
	m_Desc.MaxLOD = MaxLOD;
	m_Desc.MinLOD = MinLOD;

	if (S_OK != Core_Class::PDevice()->CreateSamplerState(&m_Desc, &m_pState))
	{
		return false;
	}
	
	return true;
}


void HSampler::Update(unsigned int _Slot)
{
	Core_Class::Context()->VSSetSamplers(_Slot, 1, &m_pState);
	Core_Class::Context()->PSSetSamplers(_Slot, 1, &m_pState);
}