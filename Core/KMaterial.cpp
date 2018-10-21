#include "KMaterial.h"
#include "Shader_Vertex.h"
#include "Shader_Pixel.h"
#include "ResourceManager.h"
#include "KMacro.h"
#include "Sampler.h"
#include "Texture.h"
#include "RenderTarget.h"


KMaterial::KMaterial() :  m_VTS(nullptr), m_PXS(nullptr), m_bOrigin(false)
{
}


KMaterial::~KMaterial()
{
}

KMaterial::KMaterial(const KMaterial& _Other)
	: Resource(_Other),
	m_VTS(_Other.m_VTS),
	m_PXS(_Other.m_PXS),
	m_Blend(_Other.m_Blend),
	m_TDVec(_Other.m_TDVec),
	m_TexMap(_Other.m_TexMap),
	m_SamMap(_Other.m_SamMap)

{
	// *this = _Mat;
}

bool KMaterial::Set_VTShader(const wchar_t* _VtxName) 
{
	m_VTS = ResourceManager<Shader_Vertex>::Find(_VtxName);
	KASSERT(nullptr == m_VTS);

	if (nullptr == m_VTS)
	{
		return false;
	}

	return true;
}

bool KMaterial::Set_PXShader(const wchar_t* _PixName) 
{
	m_PXS = ResourceManager<Shader_Pixel>::Find(_PixName);
	KASSERT(nullptr == m_PXS);

	if (nullptr == m_PXS)
	{
		return false;
	}

	return true;
}


bool KMaterial::Set_Blend(const wchar_t* _PixName) 
{
	m_Blend = ResourceManager<KBlend>::Find(_PixName);
	KASSERT(nullptr == m_Blend);

	if (nullptr == m_Blend)
	{
		return false;
	}

	return true;
}

void KMaterial::Update() 
{
	if (nullptr != m_VTS)	{ m_VTS->Update(); m_VTS->Set_Layout(); }
	if (nullptr != m_PXS) { m_PXS->Update(); }
	if (nullptr != m_Blend) { m_Blend->Update(); }
}

bool KMaterial::Create() 
{
	m_bOrigin = true;
	return true;
}


void KMaterial::Set_Tex(const KUINT& _Slot, const wchar_t* _TexName)
{
	KPtr<Texture> FindTex = ResourceManager<Texture>::Find(_TexName);
	if (nullptr == FindTex)
	{
		BBY;
	}

	std::unordered_map<KUINT, KPtr<Texture>>::iterator FindIter =  m_TexMap.find(_Slot);

	if (FindIter == m_TexMap.end())
	{
		m_TexMap.insert( std::unordered_map<KUINT, KPtr<Texture>>::value_type(_Slot, FindTex));
	}
	else 
	{
		FindIter->second = FindTex;
	}
}


void KMaterial::Set_TargetTex(const KUINT& _Slot, const wchar_t* _TexName)
{
	KPtr<RenderTarget> FindTaget = ResourceManager<RenderTarget>::Find(_TexName);
	if (nullptr == FindTaget)
	{
		BBY;
	}

	std::unordered_map<KUINT, KPtr<Texture>>::iterator FindIter = m_TexMap.find(_Slot);

	if (FindIter == m_TexMap.end())
	{
		m_TexMap.insert(std::unordered_map<KUINT, KPtr<Texture>>::value_type(_Slot, FindTaget->target_tex()));
	}
	else
	{
		FindIter->second = FindTaget->target_tex();
	}
}

void KMaterial::Set_Sampler(const KUINT& _Slot, const wchar_t* _SmpName)
{
	KPtr<Sampler> FindSmp = ResourceManager<Sampler>::Find(_SmpName);
	if (nullptr == FindSmp)
	{
		BBY;
	}

	std::unordered_map<KUINT, KPtr<Sampler>>::iterator FindIter = m_SamMap.find(_Slot);

	if (FindIter == m_SamMap.end())
	{
		m_SamMap.insert(std::unordered_map<KUINT, KPtr<Sampler>>::value_type(_Slot, FindSmp));
	}
	else
	{
		FindIter->second = FindSmp;
	}
}



void KMaterial::Update_Tex() {

	m_TSI = m_TexMap.begin();
	m_TEI = m_TexMap.end();
	for (; m_TSI != m_TEI; ++m_TSI)
	{
		m_TSI->second->Update(m_TSI->first);
	}
}

void KMaterial::Reset_Tex() 
{
	m_TSI = m_TexMap.begin();
	m_TEI = m_TexMap.end();
	for (; m_TSI != m_TEI; ++m_TSI)
	{
		m_TSI->second->Reset(m_TSI->first);
	}
}

void KMaterial::Update_Sam() 
{
	m_SSI = m_SamMap.begin();
	m_SEI = m_SamMap.end();
	for (; m_SSI != m_SEI; ++m_SSI)
	{
		m_SSI->second->Update(m_SSI->first);
	}
}

void KMaterial::Insert_TexData(TEX_TYPE _Type, const KUINT& _TexSlot, const wchar_t* _TexName, UINT _SmpSlot, const wchar_t* _SmpName)
{
	Tex_Data NewTexData;
	NewTexData.Type = (int)_Type;
	NewTexData.Tex_Idx = _TexSlot;
	NewTexData.Tex_Smp = _SmpSlot;
	m_TDVec.push_back(NewTexData);

	if (_Type == TEX_TYPE::TEX_TAGET)
	{
		Set_TargetTex(_TexSlot, _TexName);
	}
	else {
		Set_Tex(_TexSlot, _TexName);
	}

	Set_Sampler(_SmpSlot, _SmpName);
}

KUINT KMaterial::Set_TexData(Tex_Data* _Data) 
{
	for (size_t i = 0; i < m_TDVec.size(); i++)
	{
		_Data[i] = m_TDVec[i];
	}

	return (KUINT)m_TDVec.size();
}

KPtr<KMaterial> KMaterial::Clone() 
{
	return new KMaterial(*this);
}