#include "HMaterial.h"
#include "HVtxShader.h"
#include "HPixShader.h"
#include "HResMgr.h"
#include "KMacro.h"
#include "HSampler.h"
#include "HTexture.h"
#include "HRenderTaget.h"


HMaterial::HMaterial() :  m_VtxShader(nullptr), m_PixShader(nullptr), IsOri(false)
{
}


HMaterial::~HMaterial()
{
}

HMaterial::HMaterial(const HMaterial& _Other)
	: Resource(_Other),
	m_VtxShader(_Other.m_VtxShader),
	m_PixShader(_Other.m_PixShader),
	m_Blend(_Other.m_Blend),
	m_TexData(_Other.m_TexData),
	m_TexMap(_Other.m_TexMap),
	m_SmpMap(_Other.m_SmpMap)

{
	// *this = _Mat;
}

bool HMaterial::SetVtxShader(const wchar_t* _VtxName) 
{
	m_VtxShader = ResourceManager<HVtxShader>::Find(_VtxName);
	KASSERT(nullptr == m_VtxShader);

	if (nullptr == m_VtxShader)
	{
		return false;
	}

	return true;
}

bool HMaterial::SetPixShader(const wchar_t* _PixName) 
{
	m_PixShader = ResourceManager<HPixShader>::Find(_PixName);
	KASSERT(nullptr == m_PixShader);

	if (nullptr == m_PixShader)
	{
		return false;
	}

	return true;
}


bool HMaterial::SetBlend(const wchar_t* _PixName) 
{
	m_Blend = ResourceManager<HBlend>::Find(_PixName);
	KASSERT(nullptr == m_Blend);

	if (nullptr == m_Blend)
	{
		return false;
	}

	return true;
}

void HMaterial::Update() 
{
	if (nullptr != m_VtxShader)	{ m_VtxShader->Update(); m_VtxShader->SetLayout(); }
	if (nullptr != m_PixShader) { m_PixShader->Update(); }
	if (nullptr != m_Blend) { m_Blend->Update(); }
}

bool HMaterial::Create() 
{
	IsOri = true;
	return true;
}


void HMaterial::SetTex(unsigned int _Slot, const wchar_t* _TexName) 
{
	KPtr<HTexture> FindTex = ResourceManager<HTexture>::Find(_TexName);
	if (nullptr == FindTex)
	{
		BBY;
	}

	std::unordered_map<unsigned int, KPtr<HTexture>>::iterator FindIter =  m_TexMap.find(_Slot);

	if (FindIter == m_TexMap.end())
	{
		m_TexMap.insert( std::unordered_map<unsigned int, KPtr<HTexture>>::value_type(_Slot, FindTex));
	}
	else 
	{
		FindIter->second = FindTex;
	}
}


void HMaterial::SetTagetTex(unsigned int _Slot, const wchar_t* _TexName)
{
	KPtr<HRenderTaget> FindTaget = ResourceManager<HRenderTaget>::Find(_TexName);
	if (nullptr == FindTaget)
	{
		BBY;
	}

	std::unordered_map<unsigned int, KPtr<HTexture>>::iterator FindIter = m_TexMap.find(_Slot);

	if (FindIter == m_TexMap.end())
	{
		m_TexMap.insert(std::unordered_map<unsigned int, KPtr<HTexture>>::value_type(_Slot, FindTaget->TagetTex()));
	}
	else
	{
		FindIter->second = FindTaget->TagetTex();
	}
}

void HMaterial::SetSmp(unsigned int _Slot, const wchar_t* _SmpName) 
{
	KPtr<HSampler> FindSmp = ResourceManager<HSampler>::Find(_SmpName);
	if (nullptr == FindSmp)
	{
		BBY;
	}

	std::unordered_map<unsigned int, KPtr<HSampler>>::iterator FindIter = m_SmpMap.find(_Slot);

	if (FindIter == m_SmpMap.end())
	{
		m_SmpMap.insert(std::unordered_map<unsigned int, KPtr<HSampler>>::value_type(_Slot, FindSmp));
	}
	else
	{
		FindIter->second = FindSmp;
	}
}



void HMaterial::TexUpdate() {

	m_TexStartIter = m_TexMap.begin();
	m_TexEndIter = m_TexMap.end();
	for (; m_TexStartIter != m_TexEndIter; ++m_TexStartIter)
	{
		m_TexStartIter->second->Update(m_TexStartIter->first);
	}
}

void HMaterial::TexReset() 
{
	m_TexStartIter = m_TexMap.begin();
	m_TexEndIter = m_TexMap.end();
	for (; m_TexStartIter != m_TexEndIter; ++m_TexStartIter)
	{
		m_TexStartIter->second->Reset(m_TexStartIter->first);
	}
}

void HMaterial::SmpUpdate() 
{
	m_SmpStartIter = m_SmpMap.begin();
	m_SmpEndIter = m_SmpMap.end();
	for (; m_SmpStartIter != m_SmpEndIter; ++m_SmpStartIter)
	{
		m_SmpStartIter->second->Update(m_SmpStartIter->first);
	}
}

void HMaterial::AddTexData(TEXTYPE _Type, UINT _TexSlot, const wchar_t* _TexName, UINT _SmpSlot, const wchar_t* _SmpName)
{
	TexData NewTexData;
	NewTexData.Type = (int)_Type;
	NewTexData.Tex_Idx = _TexSlot;
	NewTexData.Tex_Smp = _SmpSlot;
	m_TexData.push_back(NewTexData);

	if (_Type == TEXTYPE::TT_TAGET)
	{
		SetTagetTex(_TexSlot, _TexName);
	}
	else {
		SetTex(_TexSlot, _TexName);
	}

	SetSmp(_SmpSlot, _SmpName);
}

unsigned int HMaterial::TexDataSetting(TexData* _Data) 
{
	for (size_t i = 0; i < m_TexData.size(); i++)
	{
		_Data[i] = m_TexData[i];
	}

	return (unsigned int)m_TexData.size();
}

KPtr<HMaterial> HMaterial::Clone() 
{
	return new HMaterial(*this);
}