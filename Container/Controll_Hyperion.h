#pragma once
#include <Component.h>
#include "DXContainer.h"
#include <list>


class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class Controll_Hyperion : public Component
{
public:
	Controll_Hyperion();
	~Controll_Hyperion();


public:
	enum MOVE_TYPE
	{
		MT_WARPIN = 0,
		MT_ATTACK,
		MT_WARPOUT,
	};

private:
	bool m_MirrorY;

	std::list<KPtr<Force_Unit>>					m_pUnitList;
	std::list<KPtr<Force_Unit>>::iterator		m_SULI;
	std::list<KPtr<Force_Unit>>::iterator		m_EULI;
	std::list<KVector>				m_TPos;

	MOVE_TYPE	m_MType;

	float m_ATime;
	float m_UTime;

private:
	void Update_WARPIN();
	void Update_ATTACK();
	void Update_WARPOUT();


public:
	bool Init(const KVector& _InitPos, const KVector& _RotPos, KPtr<Renderer_Terrain> _Ter);
	void Update();

};

