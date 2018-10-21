#include "HSpRenderer.h"
#include "KMacro.h"
#include "KWindow.h"
#include "ResourceManager.h"
#include "KBlend.h"
#include "Sampler.h"
#include "Texture.h"


HSpRenderer::HSpRenderer() : m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_ImgIndex(0)
{
}


HSpRenderer::~HSpRenderer()
{
}

void HSpRenderer::Render(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	if (nullptr == m_Img)
	{
		return;
	}

	// 버텍스 쉐이더가 

	SubTransUpdate();

	m_Mat->VShader()->SettingCB<KMatrix>(L"TRANS", (CSWMat() * _Camera->ViewProj()).RTranspose());
	m_Mat->PShader()->SettingCB<KVector4>(L"MULCOLOR", m_Color);
	m_Mat->PShader()->SettingCB<KVector4>(L"IMGUV", m_Img->Uv(m_ImgIndex));

	m_Mat->Update();

	m_Img->sampler()->Update(0);
	m_Img->texture()->Update(0);

	m_Mesh->Update();
	m_Mesh->Render();
}

bool HSpRenderer::Init(int _Order)
{
	Renderer::Init(_Order);

	if (false == SetMat(L"IMGMAT"))
	{
		return false;
	}
	if (false == SetMesh(L"TEXMESH")) 
	{
		return false;
	}

	return true;
}

bool HSpRenderer::Init(const wchar_t* _ImgName, int _Order) 
{
	Image(_ImgName);
	if (nullptr == m_Img)
	{
		return false;
	}

	if (false == Init(_Order))
	{
		return false;
	}

	return true;
}

void HSpRenderer::ImageScaleSetting() 
{
	if (nullptr == m_Img)
	{
		return;
	}

	KVector4 Scale = m_Img->texture()->ImageSize();

	Scale.z = 1.0f;

	SubScale(Scale);
}

void HSpRenderer::Image(const wchar_t* _ImageName) 
{
	if (nullptr == _ImageName)
	{
		m_Img = nullptr;
		return;
	}

	m_Img = ResourceManager<KImage>::Find(_ImageName);
	KASSERT(nullptr == m_Img);
}
