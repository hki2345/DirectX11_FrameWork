#include "Mouse_Col.h"
#include "ResourceManager.h"
#include "State.h"

#include "InputManager.h"
#include "Collider2D_DE.h"
#include "Core_Class.h"
#include "TheOne.h"


KPtr<Collider2D_DE> Mouse_Col::m_Col2D = nullptr;
KPtr<TheOne>		Mouse_Col::m_Mouse = nullptr;

Mouse_Col::Mouse_Col()
{
}


Mouse_Col::~Mouse_Col()
{
}


bool Mouse_Col::Init()
{
	if (nullptr == InputManager::Find_Command(L"MOUSE_LB"))
	{
		InputManager::Create_Command(L"MOUSE_LB", VK_LBUTTON);
	}

	if (nullptr == m_Col2D)
	{
		m_Mouse = Core_Class::main_state()->Create_TheOne(L"Mouse Collider2D");
		m_Col2D = m_Mouse->Add_Component<Collider2D_DE>(0);
		m_Col2D->type(COLL_TYPE::COL_POINT2D);
	}

	return true;
}

void Mouse_Col::Update()
{
	m_Mouse->Local_Pos(Core_Class::main_state()->camera()->screen_to_world(InputManager::mouse_pos()));
}

KPtr<Collider2D_DE> Mouse_Col::Mouse_Down(const MOUSE_BTN& _Value)
{
	switch (_Value)
	{
	case MOUSE_BTN::MBTN_LEFT:
		// 충돌 체크
		if (KEY_DOWN("MOUSE_LB"))
		{
			return Get_Coll2D();
		}
		break;

	case MOUSE_BTN::MBTN_RIGHT:
		if (KEY_DOWN("MOUSE_RB"))
		{
			return Get_Coll2D();
		}
		break;

	case MOUSE_BTN::MBTN_MIDDLE:
		if (KEY_DOWN("MOUSE_MB"))
		{
			return Get_Coll2D();
		}
		break;

	default:
		break;
	}

	return nullptr;
}


KPtr<Collider2D_DE> Mouse_Col::Mouse_Up(const MOUSE_BTN& _Value)
{
	switch (_Value)
	{
	case MOUSE_BTN::MBTN_LEFT:
		// 충돌 체크
		if (KEY_UP("MOUSE_LB"))
		{
			return Get_Coll2D();
		}
		break;

	case MOUSE_BTN::MBTN_RIGHT:
		if (KEY_UP("MOUSE_RB"))
		{
			return Get_Coll2D();
		}
		break;

	case MOUSE_BTN::MBTN_MIDDLE:
		if (KEY_UP("MOUSE_MB"))
		{
			return Get_Coll2D();
		}
		break;

	default:
		break;
	}

	return nullptr;
}

KPtr<Collider2D_DE> Mouse_Col::Mouse_Press(const MOUSE_BTN& _Value)
{
	switch (_Value)
	{
	case MOUSE_BTN::MBTN_LEFT:
		// 충돌 체크
		if (KEY_PRESS("MOUSE_LB"))
		{
			return Get_Coll2D();
		}
		break;

	case MOUSE_BTN::MBTN_RIGHT:
		if (KEY_PRESS("MOUSE_RB"))
		{
			return Get_Coll2D();
		}
		break;

	case MOUSE_BTN::MBTN_MIDDLE:
		if (KEY_PRESS("MOUSE_MB"))
		{
			return Get_Coll2D();
		}
		break;

	default:
		break;
	}
	return nullptr;
}

KPtr<Collider2D_DE> Mouse_Col::Get_Coll2D()
{
	// TheOne이 맞는지 체크
	if (nullptr == Core_Class::main_state() || nullptr == m_Col2D)
	{
		return nullptr;
	}

	KPtr<Collision> m_Col = Core_Class::main_state()->this_Collider2DManager.All_Collision_Check(m_Col2D);
	if (nullptr != m_Col)
	{
		return m_Col;
	}
	return nullptr;
}