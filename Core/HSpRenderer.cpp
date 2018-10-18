#include "HSpRenderer.h"
#include "KMacro.h"
#include "HWindow.h"
#include "HResMgr.h"
#include "HBlend.h"
#include "HSampler.h"
#include "HTexture.h"


HSpRenderer::HSpRenderer() : m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_ImgIndex(0)
{
}


HSpRenderer::~HSpRenderer()
{
}

void HSpRenderer::Render(KPtr<HCamera> _Camera)
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

	m_Mat->VTXSH()->SettingCB<HMAT>(L"TRANS", (CSWMat() * _Camera->VP()).RTranspose());
	m_Mat->PIXSH()->SettingCB<HVEC>(L"MULCOLOR", m_Color);
	m_Mat->PIXSH()->SettingCB<HVEC>(L"IMGUV", m_Img->Uv(m_ImgIndex));

	m_Mat->Update();

	m_Img->GetSam()->Update(0);
	m_Img->GetTex()->Update(0);

	m_Mesh->Update();
	m_Mesh->Render();
}

bool HSpRenderer::Init(int _Order)
{
	HRenderer::Init(_Order);

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

	HVEC Scale = m_Img->GetTex()->ImageSize();

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

	m_Img = HResMgr<HImage>::Find(_ImageName);
	KASSERT(nullptr == m_Img);
}
