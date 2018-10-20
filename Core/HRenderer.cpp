#include "HRenderer.h"
#include "KMacro.h"
#include "HResMgr.h"
#include "HScene.h"
#include "KDevice.h"
#include "KWindow.h"
#include "HMaterial.h"
#include "Core_Class.h"

HRenderer::HRenderer() : m_RsState(nullptr)
{
	if (false == SetMat(L"NONEMAT"))
	{
		BBY;
	}
}


HRenderer::~HRenderer()
{
}

void HRenderer::SetRSState(const wchar_t* _Name) {
	m_RsState = Window()->Device().FindRsMode(_Name);

	if (nullptr == m_RsState)
	{
		KASSERT(true);
		return;
	}
}

bool HRenderer::Init(int _Order)
{
	m_Order = _Order;
	Scene()->RenderMgr.PushRenderer(this);
	return true;
}

bool HRenderer::SetMesh(const wchar_t* _Res) {
	m_Mesh = ResourceManager<HMesh>::Find(_Res);
	KASSERT(nullptr == m_Mesh);

	if (nullptr == m_Mesh)
	{
		return false;
	}
	return true;
}

bool HRenderer::SetMat(const wchar_t* _Res)
{
	m_Mat = ResourceManager<HMaterial>::Find(_Res);
	KASSERT(nullptr == m_Mat);

	if (nullptr == m_Mat)
	{
		return false;
	}

	return true;
}

KPtr<HMaterial> HRenderer::Mat() 
{
	if (m_Mat->IsOri)
	{
		m_Mat = m_Mat->Clone();
	}

	return m_Mat;
}

void HRenderer::RenderUpdate()
{
	if (nullptr != m_RsState)
	{
		m_RsState->Update();
	}

	if (nullptr != m_Mat)
	{
		m_Mat->TexUpdate();
		m_Mat->SmpUpdate();
	}
}

void HRenderer::RenderEndUpdate() 
{
	Window()->Device().ResetRSState();
}

void HRenderer::TransUpdate(KPtr<HCamera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate(); 

	m_MatData.m_W = m_Trans->CWMat().RVTranspose();
	m_MatData.m_V = _Camera->CV().RVTranspose();
	m_MatData.m_P = _Camera->CP().RVTranspose();
	m_MatData.m_WV = (m_Trans->CWMat() * _Camera->CV()).RTranspose();
	m_MatData.m_WVP = (m_Trans->CWMat() * _Camera->VP()).RTranspose();

	TransCBUpdate();
}

void HRenderer::TransCBUpdate() 
{
	// size_t Size = sizeof(ROpt);

	if (nullptr != m_Mat)
	{
		ROpt.TexCount = m_Mat->TexDataSetting(ROpt.ArrTex);
	}

	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATDATA", m_MatData, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATDATA", m_MatData, SHTYPE::ST_PS);
	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_PS);
}

void HRenderer::MatUpdate() 
{
	if (nullptr != m_Mat)
	{
		m_Mat->Update();
	}
}

void HRenderer::MeshUpdate()
{
	if (nullptr != m_Mesh)
	{
		m_Mesh->Update();
		m_Mesh->Render();
	}
}