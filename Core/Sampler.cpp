#include "Sampler.h"
#include "Core_Class.h"
#include "KMacro.h"


Sampler::Sampler()
{
}


Sampler::~Sampler()
{
	if (nullptr != m_State) { m_State->Release(); }
}


bool Sampler::Create()
{
	if (false == Set_State())
	{
		return false;
	}
	
	return true;
}


bool Sampler::Set_State(
	D3D11_FILTER Filter,
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
	if (nullptr != m_State) { m_State->Release(); }

	m_Desc.Filter = Filter;
	m_Desc.AddressU = AddressU;
	m_Desc.AddressV = AddressV;
	m_Desc.AddressW = AddressW;

	// 해당 정보들은 float4형식으로 담기는데 어떻게 담기가 귀찮으니까
	// KColor를 가져다 쓴 것 뿐 따라서 memcpy를 해야 완벽하게 들어간다. = 사실 귀찮아서
	memcpy_s(m_Desc.BorderColor, sizeof(KColor), BorderColor.s, sizeof(KColor));

	m_Desc.ComparisonFunc = ComparisonFunc;
	m_Desc.MaxAnisotropy = MaxAnisotropy;
	m_Desc.MipLODBias = MipLODBias;
	m_Desc.MinLOD = MinLOD;
	m_Desc.MaxLOD = MaxLOD;

	if (S_OK != Core_Class::device()->CreateSamplerState(&m_Desc, &m_State))
	{
		return false;
	}

	return true;
}

void Sampler::Update()
{
	Core_Class::device_context()->PSSetSamplers(0, 1, &m_State);
}