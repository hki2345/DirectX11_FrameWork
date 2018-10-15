#include "Camera.h"
#include "TransPosition.h"
#include "TheOne.h"
#include "KWindow.h"
#include "Core_Class.h"
#include "KMacro.h"


#include "Sampler.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"


Camera::Camera() :m_eSMode(SM_WINDOW), m_ePMode(PM_ORTHGRAPHICS),
m_fFov(DirectX::XM_1DIV2PI), m_fNear(1.0f), m_fFar(1000.0f)
{
	m_Mesh = ResourceManager<Mesh>::Find(L"RECT3D");
	m_LightMat = ResourceManager<Material>::Find(L"LIGHTMERGE_MAT");
	m_ScreenMat = ResourceManager<Material>::Find(L"SCREENMERGE_MAT");
}


Camera::~Camera()
{
}

bool Camera::Init(const int& _Order /*= 0*/)
{
	m_Order = _Order;

	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return false;
	}

	m_Proj.Identity();
	m_View.Identity();

	state()->this_RenderManager.Insert_Camera(this);
	
	if (nullptr == state()->m_Camera)
	{
		state()->m_Camera = this;
	}

	m_MTarget = new RenderTarget_Multi();
	m_MTarget->Create_Target(Core_Class::Main_Window().widthu(), Core_Class::Main_Window().heigthu(),
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	return true;
}


KVector2 Camera::screen_to_world(const KVector2& _StatePos)
{
	switch (m_ePMode)
	{
	case Camera::PM_PERSPECTIVE:
		break;
	case Camera::PM_ORTHGRAPHICS:
	{
		KVector Pos;

		KVector2 WinSize = window()->size();

		float fX = _StatePos.x / WinSize.x;
		float fY = _StatePos.y / WinSize.y;

		float rX = m_ScreenSize.x * fX - m_ScreenSize.x_part();
		float rY = m_ScreenSize.y * -fY + m_ScreenSize.y_part();

		Pos.m_Pos = KVector2(rX, rY);

		Pos.m_Pos += m_Trans->World_Pos();

		return Pos;
	}
	default:
		break;
	}

	return KVector::Zero;
}


void Camera::Update()
{
}


void Camera::FinalUpdate()
{
	if (true == m_Trans->BWorld())
	{
		m_View.ViewTo_LH(m_Trans->World_Pos(), m_Trans->World_Forward(), m_Trans->World_Up());
	}

	switch (m_eSMode)
	{
	case Camera::SM_WINDOW:
		m_ScreenSize.x = window()->widthf();
		m_ScreenSize.y = window()->heigthf();
		break;
	default:
		break;
	}

	m_ScreenRatioX = m_ScreenSize.x_ratio();
	m_ScreenRatioY = m_ScreenSize.y_ratio();
	

	switch (m_ePMode)
	{
	case Camera::PM_PERSPECTIVE:
		m_Proj.Proj_Perspective(m_fFov, m_ScreenSize.x, m_ScreenSize.y, m_fNear, m_fFar);
		break;
	case Camera::PM_ORTHGRAPHICS:
		m_Proj.Proj_Orthographic(m_ScreenSize.x, m_ScreenSize.y, m_fNear, m_fFar);
		break;
	default:
		break;
	}

	m_VP = m_View * m_Proj;
}


void Camera::End_Update()
{
}


// Merge
void Camera::Render_Light()
{
	if (nullptr == m_Mesh || nullptr == m_LightMat)
	{
		KASSERT(true);
	}
	
	
	m_LightMat->Update();
	m_LightMat->Update_Tex();
	m_Mesh->Update();
	m_Mesh->Render();
	m_LightMat->Reset();


}

void Camera::Render_Screen()
{
	if (nullptr == m_Mesh || nullptr == m_LightMat)
	{
		KASSERT(true);
	}


	m_MTarget->texture(0)->Update(0);
	m_ScreenMat->Update_Tex();
	m_Mesh->Update();
	m_Mesh->Render();
	m_MTarget->texture(0)->Reset(0);
}