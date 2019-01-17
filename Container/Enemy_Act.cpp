#include "Controll_AI.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>



void Controll_AI::Update_MIDLE()
{
	if (Controll_AI::AT_DEATH == m_AType)
	{
		return;
	}



	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01);
}
void Controll_AI::Update_MOVE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::WALK01);
	m_RenderRot.y = m_PlayRot.y + KPI;
	
}
void Controll_AI::Update_RUN()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::WALK01);



	m_RenderRot.y = m_PlayRot.y + KPI;
}



void Controll_AI::Update_AIDLE()
{
	if (Controll_AI::AT_DEATH == m_AType || .0f > m_pUnit->hp())
	{
		m_AType = Controll_AI::AT_DEATH;
		return;
	}
}
void Controll_AI::Update_ATTACK01()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}
void Controll_AI::Update_ATTACK02()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}
void Controll_AI::Update_ATTACK03()
{
	m_RenderRot.y = m_PlayRot.y + KPI;
}
void Controll_AI::Update_DEATH()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::DEATH);

	if (true == m_pUnit->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}