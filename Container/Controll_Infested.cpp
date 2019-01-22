#include "Controll_Infested.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>



Controll_Infested::Controll_Infested()
{
}
Controll_Infested::~Controll_Infested()
{
}

void Controll_Infested::Update_MIDLE()
{
	Controll_AI::Update_MIDLE();
}
void Controll_Infested::Update_MOVE()
{
	Controll_AI::Update_MOVE();
}



void Controll_Infested::Update_AIDLE()
{
	Controll_AI::Update_AIDLE();
}


void Controll_Infested::Update_BURROW()
{

	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::FIDGET02);
}
void Controll_Infested::Update_HIDE()
{
	Controll_AI::Update_HIDE();
}
void Controll_Infested::Update_UNBURROW()
{
	Controll_AI::Update_UNBURROW();
}


void Controll_Infested::Update_ATTACK01()
{
	Controll_AI::Update_ATTACK01();
}
void Controll_Infested::Update_ATTACK02()
{
	Controll_AI::Update_ATTACK02();
}
void Controll_Infested::Update_ATTACK03()
{
	Controll_AI::Update_ATTACK03();
}
void Controll_Infested::Update_DEATH()
{
	m_pUnit->Reset_Renderer();
	if (nullptr == DeathRender)
	{
		DeathRender = one()->Add_Component<Renderer_BonAni>();
		DeathRender->Set_Fbx(L"InfestedMarineDeath.KM3");
		DeathRender->Create_Animation(false);
		DeathRender->Set_Clip(0);
	}

	if (true == DeathRender->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}