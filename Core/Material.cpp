#include "Material.h"
#include "ResourceManager.h"
#include "KMacro.h"

Material::Material()
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
	return true;
}