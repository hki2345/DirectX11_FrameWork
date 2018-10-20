#include "HCamera.h"
#include "HTrans.h"
#include "HActor.h"
#include "KWindow.h"
#include "Core_Class.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "HResMgr.h"
#include "HMesh.h"
#include "HMaterial.h"

HCamera::HCamera() : m_eSMode(SM_WINDOW), m_ePMode(PM_ORTH), m_fFov(3.14f * 0.5f), m_fNear(1.0f), m_fFar(1000.0f), m_Order(0)
{
	m_CamMesh = ResourceManager<HMesh>::Find(L"RECT");
	m_CamMat = ResourceManager<HMaterial>::Find(L"DEFFERDMERGEMAT");
	m_CamScreenMat = ResourceManager<HMaterial>::Find(L"SCREENMERGEMAT");
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
	m_CameraTaget->CreateTaget(Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	return true;
}

void HCamera::Update() 
{
}

KVector4 HCamera::ScreenToWorld(KVector2 _ScreenPos)
{
	switch (m_ePMode)
	{
	case HCamera::PM_PERS:
		break;
	case HCamera::PM_ORTH:
	{
		KVector4 Pos;

		KVector2 WinSize = Window()->size();
		//         1024               200
		float fX = _ScreenPos.x / WinSize.x;
		float fY = _ScreenPos.y / WinSize.y;

		//         600
		float rX = m_ScreenSize.x * fX - m_ScreenSize.x_part();
		float rY = m_ScreenSize.y * -fY + m_ScreenSize.y_part();

		Pos.m_Pos = KVector2(rX, rY);

		Pos.m_Pos += m_Trans->WPos();

		return Pos;
	}
	default:
		break;
	}

	return KVector4::Zero;
}

void HCamera::FinalUpdate()
{
	m_View.ViewToLH(m_Trans->WPos(), m_Trans->WForward(), m_Trans->WUp());

	switch (m_eSMode)
	{
	case HCamera::SM_WINDOW:
		m_ScreenSize.x = Window()->width_f();
		m_ScreenSize.y = Window()->height_f();
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
		BBY;
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
		BBY;
	}

	m_CameraTaget->TagetTex(0)->Update(0);
	m_CamScreenMat->Update();
	m_CamMesh->Update();
	m_CamMesh->Render();
	m_CameraTaget->TagetTex(0)->Reset(0);
}