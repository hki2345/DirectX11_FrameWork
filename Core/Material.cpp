#include "Material.h"
#include "ResourceManager.h"
#include "KMacro.h"

#include "Sampler.h"
#include "Texture.h"

Material::Material(): m_Vertex_Shader(nullptr), m_Pixel_Shader(nullptr), Check_Original(false)
{
}

Material::Material(const Material& _Other) :
	KResource(_Other),
	m_Vertex_Shader(_Other.m_Vertex_Shader),
	m_Pixel_Shader(_Other.m_Pixel_Shader),
	m_Blend(_Other.m_Blend),
	m_TD_Vec(_Other.m_TD_Vec),
	m_TexMap(_Other.m_TexMap),
	m_SamMap(_Other.m_SamMap)
{

}


Material::~Material()
{
}


bool Material::Set_VShader(const wchar_t* _Name)
{
	m_Vertex_Shader = ResourceManager<Vertex_Shader>::Find(_Name);
	KASSERT(nullptr == m_Vertex_Shader);
	if (nullptr == m_Vertex_Shader)
	{
		return false;
	}
	return true;
}

bool Material::Set_PShader(const wchar_t* _Name)
{
	m_Pixel_Shader = ResourceManager<Pixel_Shader>::Find(_Name);
	KASSERT(nullptr == m_Pixel_Shader);
	if (nullptr == m_Pixel_Shader)
	{
		return false;
	}
	return true;
}

bool Material::Set_Blend(const wchar_t* _Name)
{
	m_Blend = ResourceManager<Blend>::Find(_Name);
	KASSERT(nullptr == m_Blend);
	if (nullptr == m_Blend)
	{
		return false;
	}
	return true;
}


void Material::Update()
{
	if (nullptr != m_Vertex_Shader)
	{
		m_Vertex_Shader->Update();
		m_Vertex_Shader->Set_LayOut();
	}
	if (nullptr != m_Pixel_Shader)
	{
		m_Pixel_Shader->Update();
	}
}

bool Material::Create()
{
	Check_Original = true;
	return true;
}




void Material::texture(const UINT& _Slot, const wchar_t* _Name)
{
	KPtr<Texture> Find = ResourceManager<Texture>::Find(_Name);
	KASSERT(nullptr == Find);

	std::unordered_map<UINT, KPtr<Texture>>::iterator FI = m_TexMap.find(_Slot);

	if (FI == m_TexMap.end())
	{
		m_TexMap.insert(std::unordered_map<UINT, KPtr<Texture>>::value_type(_Slot, Find));
	}
	else
	{
		FI->second = Find;
	}
}
void Material::sampler(const UINT& _Slot, const wchar_t* _Name)
{
	KPtr<Sampler> Find = ResourceManager<Sampler>::Find(_Name);
	KASSERT(nullptr == Find);

	std::unordered_map<UINT, KPtr<Sampler>>::iterator FI = m_SamMap.find(_Slot);

	if (FI == m_SamMap.end())
	{
		m_SamMap.insert(std::unordered_map<UINT, KPtr<Sampler>>::value_type(_Slot, Find));
	}
	else
	{
		FI->second = Find;
	}
}


void Material::insert_TD(UINT _TexSlot, const wchar_t* _TexName, const UINT& _SmSlot /*= 0*/,
	const wchar_t* _SmName/* = L"DefaultSam"*/)
{
	Texture_Data NewData;
	NewData.TInx = _TexSlot;
	NewData.TSmp = _SmSlot;
	m_TD_Vec.push_back(NewData);
	texture(_TexSlot, _TexName);
	sampler(_SmSlot, _SmName);
}


UINT Material::texture_data(Texture_Data* _Data)
{
	for (size_t i = 0; i < m_TD_Vec.size(); i++)
	{
		_Data[i] = m_TD_Vec[i];
	}

	return (UINT)m_TD_Vec.size();
}


void Material::Update_Tex()
{
	m_TSI = m_TexMap.begin();
	m_TEI = m_TexMap.end();

	for (; m_TSI != m_TEI; ++m_TSI)
	{
		m_TSI->second->Update(m_TSI->first);
	}
}
void Material::Update_Sam()
{
	m_SSI = m_SamMap.begin();
	m_SEI = m_SamMap.end();

	for (; m_SSI != m_SEI; ++m_SSI)
	{
		m_SSI->second->Update(m_SSI->first);
	}
}