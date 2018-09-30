#include "Camera.h"
#include "TransPosition.h"
#include "TheOne.h"
#include "KWindow.h"
#include "Core_Class.h"
#include "KMacro.h"


Camera::Camera() :m_eSMode(SM_WINDOW), m_ePMode(PM_ORTHGRAPHICS),
m_fFov(DirectX::XM_1DIV2PI), m_fNear(1.0f), m_fFar(1000.0f)
{
}


Camera::~Camera()
{
}

bool Camera::Init()
{
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