#include "HCamera.h"
#include "HTrans.h"
#include "HActor.h"
#include "HWindow.h"
#include "HVAR.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "HResMgr.h"
#include "HMesh.h"
#include "HMaterial.h"

HCamera::HCamera() : m_eSMode(SM_WINDOW), m_ePMode(PM_ORTH), m_fFov(3.14f * 0.5f), m_fNear(1.0f), m_fFar(1000.0f), m_Order(0)
{
	m_CamMesh = HResMgr<HMesh>::Find(L"RECT");
	m_CamMat = HResMgr<HMaterial>::Find(L"DEFFERDMERGEMAT");
	m_CamScreenMat = HResMgr<HMaterial>::Find(L"SCREENMERGEMAT");
}

HCamera::~HCamera()
{
}

bool HCamera::Init(int _Order /*= 0*/)
{
	if (nullptr == m_Trans)
	{
		return false;
	}

	m_Order = _Order;

	m_Proj.Iden();
	m_View.Iden();

	Scene()->RenderMgr.PushCamera(this);

	if (nullptr == Scene()->m_Camera) {
		Scene()->m_Camera = this;
	}
	
	m_CameraTaget = new HMultiRenderTaget();
	m_CameraTaget->CreateTaget(HVAR::MainWindow().UWidth(), HVAR::MainWindow().UHeight(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	return true;
}

void HCamera::Update() 
{
}

HVEC HCamera::ScreenToWorld(HVEC2 _ScreenPos)
{
	switch (m_ePMode)
	{
	case HCamera::PM_PERS:
		break;
	case HCamera::PM_ORTH:
	{
		HVEC Pos;

		HVEC2 WinSize = Window()->Size();
		//         1024               200
		float fX = _ScreenPos.x / WinSize.x;
		float fY = _ScreenPos.y / WinSize.y;

		//         600
		float rX = m_ScreenSize.x * fX - m_ScreenSize.HX();
		float rY = m_ScreenSize.y * -fY + m_ScreenSize.HY();

		Pos.m_Pos = HVEC2(rX, rY);

		Pos.m_Pos += m_Trans->WPos();

		return Pos;
	}
	default:
		break;
	}

	return HVEC::ZERO;
}

void HCamera::FinalUpdate()
{
	m_View.ViewToLH(m_Trans->WPos(), m_Trans->WForward(), m_Trans->WUp());

	switch (m_eSMode)
	{
	case HCamera::SM_WINDOW:
		m_ScreenSize.x = Window()->FWidth();
		m_ScreenSize.y = Window()->FHeight();
		break;
	default:
		break;
	}

	if (0 < m_ScreenSize.y)
	{
		m_ScreenRatioX = m_ScreenSize.x / m_ScreenSize.y;
	}
	if (0 < m_ScreenSize.x)
	{
		m_ScreenRatioY = m_ScreenSize.y / m_ScreenSize.x;
	}

	switch (m_ePMode)
	{
	case HCamera::PM_PERS:
		m_Proj.PersLH(m_fFov, m_ScreenSize.x, m_ScreenSize.y, m_fNear, m_fFar);
		break;
	case HCamera::PM_ORTH:
		m_Proj.OrthLH(m_ScreenSize.x, m_ScreenSize.y, m_fNear, m_fFar);
		break;
	default:
		break;
	}

	m_VP = m_View * m_Proj;

}

void HCamera::EndUpdate() 
{
	
}

void HCamera::LightMerge()
{
	if (nullptr == m_CamMesh || nullptr == m_CamMat)
	{
		BOOM;
	}

	m_CamMat->Update();
	m_CamMat->TexUpdate();
	m_CamMesh->Update();
	m_CamMesh->Render();
	m_CamMat->TexReset();

}

void HCamera::ScreenMerge()
{
	if (nullptr == m_CamMesh || nullptr == m_CamScreenMat)
	{
		BOOM;
	}

	m_CameraTaget->TagetTex(0)->Update(0);
	m_CamScreenMat->Update();
	m_CamMesh->Update();
	m_CamMesh->Render();
	m_CameraTaget->TagetTex(0)->Reset(0);
}