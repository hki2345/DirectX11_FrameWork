#include "Controll_Hyperion.h"
#include "Force_Unit.h"
#include "SC2_Force.h"
#include "Con_Class.h"

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>


#include <SoundPlayer.h>


Controll_Hyperion::Controll_Hyperion()
{
}


Controll_Hyperion::~Controll_Hyperion()
{
	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
	{
		(*m_SULI)->one()->Set_Death();
	}

	m_pUnitList.clear();
}



bool Controll_Hyperion::Init(
	const KVector& _InitPos
	, const KVector& _Forward
	, const KVector& _Right
	, const KVector& _RotPos
	, KPtr<Renderer_Terrain> _Ter)
{
	m_For = _Forward;

	m_pUnitList.push_back(Con_Class::s2_manager()->Find_Force(L"LUV")->Create_Unit(L"HYPERION", _Ter, state()));
	for (size_t i = 0; i < 8; i++)
	{
		m_pUnitList.push_back(Con_Class::s2_manager()->Find_Force(L"LUV")->Create_Unit(L"BATTLECRUISER", _Ter, state()));
	}

	m_SULI = m_pUnitList.begin();
	(*m_SULI)->one()->Trans()->scale_local(KVector(3.0f, 3.0f, 3.0f));

	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (; m_SULI != m_EULI; ++m_SULI)
	{
		(*(*m_SULI)->list_renderer().begin())->rot_pivot(_RotPos);
	}




	m_TPos.push_back(KVector(_InitPos + _Forward * 13.0f + KVector(.0f, 9.0f, .0f)));
	
	m_TPos.push_back(KVector(_InitPos + _Forward * 14.0f + _Right * 4.0f + KVector(.0f, 8.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * 15.0f + _Right * 10.0f + KVector(.0f, 10.0f, .0f)));
	
	m_TPos.push_back(KVector(_InitPos + _Forward * 14.0f + _Right * -4.0f + KVector(.0f, 8.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * 15.0f + _Right * -10.0f + KVector(.0f, 10.0f, .0f)));
	
	m_TPos.push_back(KVector(_InitPos + _Forward * 7.0f + _Right * 6.5f + KVector(.0f, 10.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * 5.0f + _Right * 12.0f + KVector(.0f, 8.0f, .0f)));
	
	m_TPos.push_back(KVector(_InitPos + _Forward * 7.0f + _Right * -6.5f + KVector(.0f, 10.0f, .0f)));
	m_TPos.push_back(KVector(_InitPos + _Forward * 5.0f + _Right * -12.0f + KVector(.0f, 8.0f, .0f)));



	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
	{
		(*m_SULI)->one()->Trans()->pos_local((*m_SPI) + _Forward * -100.0f);
	}


	m_MType = MOVE_TYPE::MT_COME;
	m_UTime = .0f;
	m_LauCnt = 1;

	SoundPlayer TT = SoundPlayer();
	TT.Play(L"Battlecruiser_What00.ogg");

	m_Battle = false;
	return true;
}


void Controll_Hyperion::Update()
{
	switch (m_MType)
	{
	case Controll_Hyperion::MT_COME:
		Update_COME();
		break;
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

void Controll_Hyperion::Update_COME()
{
	m_UTime += DELTATIME;
	if (2.5f <= m_UTime && false == m_Battle)
	{
		SoundPlayer TT = SoundPlayer();
		TT.Play(L"Battlecruiser_Yes01.ogg");

		m_Battle = true;
	}

	if (7.0f <= m_UTime)
	{
		SoundPlayer TT = SoundPlayer();
		TT.Play(L"Battlecruiser_HyperspaceIn01.wav");


		m_MType = MOVE_TYPE::MT_WARPIN;
		m_UTime = .0f;
	}
}

void Controll_Hyperion::Update_WARPIN()
{
	m_UTime += DELTATIME;
	
	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();
	
	
	if (m_LauCnt > 8)
	{
		m_SPI = m_TPos.begin();

		if ((*m_SULI)->one()->Trans()->pos_local().x >= (*m_SPI).x ||
			(*m_SULI)->one()->Trans()->pos_local().y >= (*m_SPI).y)
		{
			(*m_SULI)->one()->Trans()->pos_local((*m_SPI));
			m_UTime = .0f;
			m_ATime = .0f;
			m_MType = MOVE_TYPE::MT_ATTACK;


			SoundPlayer S1 = SoundPlayer();
			S1.Play(L"Fleet_Attack.mp3");
			SoundPlayer S2 = SoundPlayer();
			S2.Play(L"AC_Alarm_AirRaid_Siren.ogg");
		}
		else
		{
			(*m_SULI)->one()->Trans()->Moving(m_For * 20.0f);
		}
	}
	else
	{
		int Cnt = 0;
		for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI, ++Cnt)
		{
			if ((*m_SULI)->one()->Trans()->pos_local().x >= (*m_SPI).x ||
				(*m_SULI)->one()->Trans()->pos_local().y >= (*m_SPI).y)
			{
				++m_LauCnt;
				(*m_SULI)->one()->Trans()->pos_local((*m_SPI));
			}

			if (Cnt == m_LauCnt)
			{
				(*m_SULI)->one()->Trans()->Moving(m_For * 20.0f);
			}
		}
	}


}


void Controll_Hyperion::Update_ATTACK()
{
	m_UTime += DELTATIME;
	m_ATime += DELTATIME;


	if (m_UTime > .05f)
	{
		m_UTime = .0f;

		std::list<KPtr<Force_Unit>>::iterator S = Con_Class::force_enemy()->unit_list()->begin();
		std::list<KPtr<Force_Unit>>::iterator E = Con_Class::force_enemy()->unit_list()->end();


		m_SULI = m_pUnitList.begin();
		m_EULI = m_pUnitList.end();

		for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
		{
			(*m_SULI)->one()->Trans()->Moving(m_For * .05f);
		}

		for (; S != E; ++S)
		{
			(*S)->Damage(1.0f);
		}


		SoundPlayer S1 = SoundPlayer();
		S1.Play(L"Battlecruiser_AttackLaunch0.wav", .1f);
	}

	if (m_ATime > 20.0f)
	{
		m_MType = MOVE_TYPE::MT_WARPOUT;

		SoundPlayer TT = SoundPlayer();
		TT.Play(L"Battlecruiser_HyperspaceOut01.wav");
	}
}


void Controll_Hyperion::Update_WARPOUT()
{
	m_UTime += DELTATIME;

	m_SULI = m_pUnitList.begin();
	m_EULI = m_pUnitList.end();

	for (m_SPI = m_TPos.begin(); m_SULI != m_EULI; ++m_SULI, ++m_SPI)
	{
		(*m_SULI)->one()->Trans()->Moving(m_For * 20.0f);
	}

	if (3.0f <= m_UTime)
	{
		m_SULI = m_pUnitList.begin();
		m_EULI = m_pUnitList.end();

		for (; m_SULI != m_EULI; ++m_SULI)
		{
			(*m_SULI)->force()->Delete_Unit((*m_SULI));
			one()->Set_Death();
		}

		m_pUnitList.clear();
		m_TPos.clear();
	}
}