#include "Controll_Odin.h"
#include "Force_Unit.h"
#include "SC2_Force.h"
#include "Con_Class.h"

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>


Controll_Odin::Controll_Odin()
{
}


Controll_Odin::~Controll_Odin()
{
}



void Controll_Odin::Update_LAND()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01, false);

	if (m_TPos.y < m_pUnit->one()->Trans()->pos_local().y)
	{
		m_Gravity += 1.0f;
		m_pUnit->one()->Trans()->Moving(KVector(.0f, -m_Gravity, .0f) * DELTATIME);
	}
	else
	{
		m_UTime = .0f;
		m_MType = MOVE_TYPE::MT_ATTACK;
	}
}


void Controll_Odin::Update_ATTACK()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK01, false);
	m_UTime += DELTATIME;

	if (15.0f <= m_UTime)
	{
		m_MType = MOVE_TYPE::MT_RISE;
		m_TPos.y += 10.0f;
	}

	if (0 == m_pEnemyList.size() && true == m_pUnit->Check_AniDone())
	{
		m_MType = MOVE_TYPE::MT_RISE;
		m_TPos.y += 10.0f;
	}

	if (m_UTime > .1f)
	{
		if (0 >= m_pEnemyList.size())
		{
			return;
		}

		m_UTime = .0f;
		std::list<KPtr<Force_Unit>>::iterator S = m_pEnemyList.begin();

		(*S)->Damage(10.0f);

		if (true == (*S)->Is_HPDeath())
		{
			m_pEnemyList.erase(S);
		}
	}

}


void Controll_Odin::Update_RISE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND01, false);

	if (m_TPos.y > m_pUnit->one()->Trans()->pos_local().y)
	{
		m_pUnit->one()->Trans()->Moving(KVector(.0f, 2.5f, .0f) * DELTATIME);
	}
	else
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}


bool Controll_Odin::Init(const KVector& _InitPos, const KVector& _Rot, KPtr<Renderer_Terrain> _Ter)
{
	m_pUnit = Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->Create_Unit(L"ODIN", _Ter, state());
	(*m_pUnit->list_renderer().begin())->rot_pivot(_Rot);
	m_pUnit->Rot_Unit(_Rot);


	KVector TT = _InitPos;
	m_TPos = TT;


	TT.y += 50.0f;
	m_Gravity = 2.5f;
	m_pUnit->one()->Trans()->pos_local(TT);

	m_MType = MOVE_TYPE::MT_LAND;



	int Limit = 15;
	int Cnt = 0;

	std::list<KPtr<Force_Unit>> TList = *Con_Class::force_enemy()->unit_list();
	std::list<KPtr<Force_Unit>>::iterator S = TList.begin();
	std::list<KPtr<Force_Unit>>::iterator E = TList.end();

	float LimitRange = 10;


	for (size_t i = 0; i < 4; i++)
	{
		std::list<KPtr<Force_Unit>>::iterator S = TList.begin();
		std::list<KPtr<Force_Unit>>::iterator E = TList.end();

		for (; S != E; ++S)
		{
			if (LimitRange < m_pUnit->one()->Trans()->pos_local().distance((*S)->one()->Trans()->pos_local()) && 
				m_pEnemyList.size() <= Limit)
			{
				m_pEnemyList.push_back((*S));
			}
			else if (m_pEnemyList.size() > Limit)
			{
				return true;
			}
		}

		LimitRange -= 1;
	}

	m_UTime = .0f;
	return true;
}


void Controll_Odin::Update()
{
	switch (m_MType)
	{
	case Controll_Odin::MT_ATTACK:
		Update_ATTACK();
		break;
	case Controll_Odin::MT_LAND:
		Update_LAND();
		break;
	case Controll_Odin::MT_RISE:
		Update_RISE();
		break;
	default:
		break;
	}
}