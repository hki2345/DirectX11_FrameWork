#include "Controll_Hyperion.h"
#include "Force_Unit.h"
#include "SC2_Force.h"
#include "Con_Class.h"

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>




Controll_Hyperion::Controll_Hyperion()
{
}


Controll_Hyperion::~Controll_Hyperion()
{
}



bool Controll_Hyperion::Init(const KVector& _InitPos, const KVector& _RotPos, KPtr<Renderer_Terrain> _Ter)
{
	m_pUnitList.push_back(Con_Class::s2_manager()->Find_Force(L"LUV")->Create_Unit(L"HYPERION", _Ter, state()));
	for (size_t i = 0; i < 8; i++)
	{
		m_pUnitList.push_back(Con_Class::s2_manager()->Find_Force(L"LUV")->Create_Unit(L"BATTLECRUISER", _Ter, state()));
	}

	m_SULI = m_pUnitList.begin();
	(*m_SULI)->one()->Trans()->scale_local(KVector(2.0f, 2.0f, 2.0f));


/*

	(*m_pUnit->list_renderer().begin())->rot_pivot(_RotPos);
	m_pUnit->one()->Trans()->rotate_local(_RotPos);
	m_pUnit->one()->Trans()->pos_local(m_TPos - KVector(100.0f, 100.0f, .0f));
*/

	m_MType = MOVE_TYPE::MT_WARPIN;
	m_UTime = .0f;
	return true;
}


void Controll_Hyperion::Update()
{
	switch (m_MType)
	{
	case Controll_Hyperion::MT_WARPIN:
		Update_WARPIN();
		break;
	case Controll_Hyperion::MT_ATTACK:
		Update_ATTACK();
		break;
	case Controll_Hyperion::MT_WARPOUT:
		Update_WARPOUT();
		break;

	default:
		break;
	}
}


void Controll_Hyperion::Update_WARPIN()
{
	m_ATime = .0f;


}
void Controll_Hyperion::Update_ATTACK()
{/*
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
	m_UTime += DELTATIME;
	m_ATime += DELTATIME;


	if (m_UTime > .05f)
	{
		m_UTime = .0f;

		std::list<KPtr<Force_Unit>>::iterator S = Con_Class::force_enemy()->unit_list()->begin();
		std::list<KPtr<Force_Unit>>::iterator E = Con_Class::force_enemy()->unit_list()->end();

		for (; S != E; ++S)
		{
			(*S)->Damage(10.0f);
		}
	}

	if (m_ATime > 10.0f)
	{
		m_MType = MOVE_TYPE::MT_WARPOUT;
	}*/
}


void Controll_Hyperion::Update_WARPOUT()
{
}