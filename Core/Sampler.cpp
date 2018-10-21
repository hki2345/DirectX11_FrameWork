#include "Sampler.h"
#include "Core_Class.h"


Sampler::Sampler()
{
}


Sampler::~Sampler()
{
	if (nullptr != m_pState)
	{
		m_pState->Release();
	}
}


bool Sampler::Create() 
{
	if (false == Set_SamState())
	{
		return false;
	}

	return true;
}


bool Sampler::Set_SamState(D3D11_FILTER Filter,
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
	if (nullptr != m_pState) { m_pState->Release(); }

	m_Desc.Filter = Filter;
	m_Desc.AddressU = AddressU;
	m_Desc.AddressV = AddressV;
	m_Desc.AddressW = AddressW;

	// �ش� �������� float4�������� ���µ� ��� ��Ⱑ �������ϱ�
	// KColor�� ������ �� �� �� ���� memcpy�� �ؾ� �Ϻ��ϰ� ����. = ��� �����Ƽ� �׷��� �ǻ��� ��-��
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


void Sampler::Update(unsigned int _Slot)
{
	Core_Class::Context()->VSSetSamplers(_Slot, 1, &m_pState);
	Core_Class::Context()->PSSetSamplers(_Slot, 1, &m_pState);
}