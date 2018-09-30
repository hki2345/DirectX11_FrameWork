#include "FreeCam.h"
#include <InputManager.h>
#include <TimeManager.h>
#include <DebugManager.h>
#include <Core_Class.h>

FreeCam::FreeCam()
{
}


FreeCam::~FreeCam()
{
}

bool FreeCam::Init()
{
	m_Speed = 100.0f;
	m_RotateSpd = 50.0f;
	m_Cam = Get_Component<Camera>();
	m_Cam->Mode(Camera::PROJ_MODE::PM_PERSPECTIVE);

	return true;
}
void FreeCam::Update()
{
	if (false == Update_ScrCheck())
	{
		return;
	}

	if (true == KEY_PRESS("Left"))
	{
		m_Trans->Moving(m_Trans->Local_Left() * m_Speed);
	}

	if (true == KEY_PRESS("Right"))
	{
		m_Trans->Moving(m_Trans->Local_Right() *  m_Speed);
	}

	if (true == KEY_PRESS("Up"))
	{
		m_Trans->Moving(m_Trans->Local_Up() * m_Speed);
	}

	if (true == KEY_PRESS("Down"))
	{
		m_Trans->Moving(m_Trans->Local_Down() * m_Speed);
	}

	if (true == KEY_PRESS("Forward"))
	{
		m_Trans->Moving(m_Trans->Local_Forward()  * m_Speed);
	}

	if (true == KEY_PRESS("Back"))
	{
		m_Trans->Moving(m_Trans->Local_Back() * m_Speed);
	}

	if (true == KEY_PRESS("Cam Rotate"))
	{
		m_Trans->Rotating(KVector(
			InputManager::mouse_dir().y * m_RotateSpd, 
			InputManager::mouse_dir().x * m_RotateSpd));
	}


	KVector2 CurMouse = InputManager::mouse_pos();

	WLOG(L"RotScale : %f, %f", m_CurRot.x, m_CurRot.y);
	WLOG(L"MousePos : %f, %f", CurMouse.x, CurMouse.y);
	WLOG(L"CameraRot : %f, %f", m_Trans->Local_Rotate().x, m_Trans->Local_Rotate().y);

}

bool FreeCam::Update_ScrCheck()
{
	KVector2 MPos = InputManager::mouse_pos();
	bool Check;

	if (window()->size() > MPos && KVector2::Zero < MPos)
	{
		Check = true;
	}
	else
	{
		Check = false;
	}

	WLOG(L"Mouse InScreen : %b", Check);

	return Check;
}