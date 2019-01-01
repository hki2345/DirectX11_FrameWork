#include "Controll_User.h"
#include "Force_Unit.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>



void Controll_User::Update_MIDLE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01);

	if (true == KEY_DOWN(L"MFOR") || true == KEY_DOWN(L"MBACK") || 
		true == KEY_DOWN(L"MLEFT") || true == KEY_DOWN(L"MRIGHT"))
	{
		m_MType = Controll_User::MT_MOVE;
	}
}
void Controll_User::Update_MOVE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::WALK01);
	if (true == KEY_DOWN(L"RUN"))
	{
		m_MType = Controll_User::MT_RUN;
		return;
	}

	KPtr<TransPosition> TT = one()->Trans();
	if (true == KEY_PRESS(L"MFOR"))
	{
		TT = one()->Trans();
		TT->Moving(TT->forward_local() * m_pUnit->linear_speed() * DELTATIME);
		m_RenderRot.y = m_PlayRot.y + KPI;
	}
	if (true == KEY_PRESS(L"MBACK"))
	{
		TT = one()->Trans();
		TT->Moving(TT->back_local() * m_pUnit->linear_speed() * DELTATIME);
		m_RenderRot.y = m_PlayRot.y + KPI;
	}
	if (true == KEY_PRESS(L"MLEFT"))
	{
		TT = one()->Trans();
		TT->Moving(TT->left_local() * m_pUnit->linear_speed() * DELTATIME);

	}
	if (true == KEY_PRESS(L"MRIGHT"))
	{
		TT = one()->Trans();
		TT->Moving(TT->right_local() * m_pUnit->linear_speed() * DELTATIME);
	}

	// 종료
	if (true == KEY_UNPRESS(L"MFOR") && true == KEY_UNPRESS(L"MBACK") &&
		true == KEY_UNPRESS(L"MLEFT") && true == KEY_UNPRESS(L"MRIGHT"))
	{
		m_MType = Controll_User::MT_IDLE;
		return;
	}

	KVector TVec = TT->pos_local();
	TVec.y = m_pTer->Y_Terrain(TVec);
	TT->pos_local(TVec);
}
void Controll_User::Update_RUN()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::WALK01);


	float RunSpd = 1.5f;
	if (true == KEY_UNPRESS(L"RUN"))
	{
		m_MType = Controll_User::MT_MOVE;
		return;
	}


	KPtr<TransPosition> TT = one()->Trans();
	if (true == KEY_PRESS(L"MFOR"))
	{
		TT->Moving(TT->forward_local() * m_pUnit->linear_speed() * DELTATIME * RunSpd);
	}
	if (true == KEY_PRESS(L"MBACK"))
	{
		TT->Moving(TT->back_local() * m_pUnit->linear_speed() * DELTATIME* RunSpd);
	}
	if (true == KEY_PRESS(L"MLEFT"))
	{
		TT->Moving(TT->left_local() * m_pUnit->linear_speed() * DELTATIME * RunSpd);
	}
	if (true == KEY_PRESS(L"MRIGHT"))
	{
		TT->Moving(TT->right_local() * m_pUnit->linear_speed() * DELTATIME * RunSpd);
	}

	// 종료
	if (true == KEY_UNPRESS(L"MFOR") && true == KEY_UNPRESS(L"MBACK") &&
		true == KEY_UNPRESS(L"MLEFT") && true == KEY_UNPRESS(L"MRIGHT"))
	{
		m_MType = Controll_User::MT_IDLE;
		return;
	}

	KVector TVec = TT->pos_local();
	TVec.y = m_pTer->Y_Terrain(TVec);
	TT->pos_local(TVec);

	m_RenderRot.y = m_PlayRot.y + KPI;
}



void Controll_User::Update_AIDLE()
{

}
void Controll_User::Update_ATTACK()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}
void Controll_User::Update_BOMB()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}
void Controll_User::Update_HEAL()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}
void Controll_User::Update_OPTI()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}