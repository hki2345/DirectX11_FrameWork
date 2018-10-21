#include "Renderer.h"
#include "KMacro.h"
#include "ResourceManager.h"
#include "State.h"
#include "KDevice.h"
#include "KWindow.h"
#include "KMaterial.h"
#include "Core_Class.h"

Renderer::Renderer() : m_RsState(nullptr)
{
	if (false == SetMat(L"NONEMAT"))
	{
		BBY;
	}
}


Renderer::~Renderer()
{
}

void Renderer::SetRSState(const wchar_t* _Name) {
	m_RsState = Window()->Device().Find_RS(_Name);

	if (nullptr == m_RsState)
	{
		KASSERT(true);
		return;
	}
}

bool Renderer::Init(int _Order)
{
	m_Order = _Order;
	state()->RenderMgr.Insert_Renderer(this);
	return true;
}

bool Renderer::SetMesh(const wchar_t* _Res) {
	m_Mesh = ResourceManager<KMesh>::Find(_Res);
	KASSERT(nullptr == m_Mesh);

	if (nullptr == m_Mesh)
	{
		return false;
	}
	return true;
}

bool Renderer::SetMat(const wchar_t* _Res)
{
	m_Mat = ResourceManager<KMaterial>::Find(_Res);
	KASSERT(nullptr == m_Mat);

	if (nullptr == m_Mat)
	{
		return false;
	}

	return true;
}

KPtr<KMaterial> Renderer::material() 
{
	if (m_Mat->m_bOrigin)
	{
		m_Mat = m_Mat->Clone();
	}

	return m_Mat;
}

void Renderer::RenderUpdate()
{
	if (nullptr != m_RsState)
	{
		m_RsState->Update();
	}

	if (nullptr != m_Mat)
	{
		m_Mat->Update_Tex();
		m_Mat->Update_Sam();
	}
}

void Renderer::RenderFinalUpdate() 
{
	Window()->Device().Reset_RS();
}

void Renderer::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate(); 

	m_MD.m_W = m_Trans->worldmat_const().RVTranspose();
	m_MD.m_V = _Camera->View().RVTranspose();
	m_MD.m_P = _Camera->Proj().RVTranspose();
	m_MD.m_WV = (m_Trans->worldmat_const() * _Camera->View()).RTranspose();
	m_MD.m_WVP = (m_Trans->worldmat_const() * _Camera->ViewProj()).RTranspose();

	Update_TransCB();
}

void Renderer::Update_TransCB() 
{
	// size_t Size = sizeof(ROpt);

	if (nullptr != m_Mat)
	{
		ROpt.TexCnt = m_Mat->Set_TexData(ROpt.TexArr);
	}

	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_PS);
	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_PS);
}

void Renderer::Update_Material() 
{
	if (nullptr != m_Mat)
	{
		m_Mat->Update();
	}
}

void Renderer::Update_Mesh()
{
	if (nullptr != m_Mesh)
	{
		m_Mesh->Update();
		m_Mesh->Render();
	}
}