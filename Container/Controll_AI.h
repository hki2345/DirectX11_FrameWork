#pragma once
#include <Component.h>
#include "DXContainer.h"



class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class Controll_AI : public Component
{
public:
	Controll_AI();
	~Controll_AI();


public:
	enum MOVE_TYPE
	{
		MT_IDLE = 0,
		MT_MOVE,
	};

	enum ACT_TYPE
	{
		AT_IDLE = 0,


		AT_BURROW,
		AT_HIDE,
		AT_UNBURROW,

		AT_ATTACK01,
		AT_ATTACK02,
		AT_ATTACK03,
		
		AT_DEATH,
	};

private:
	bool m_Attack;
	float m_UTime;
	float m_ATime;

	float m_MRange;
	float m_ARange;

	std::list<KPtr<Renderer_BonAni>> m_RList;
	std::list<KPtr<Renderer_BonAni>>::iterator m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator m_ERI;

	KPtr<Force_Unit>		m_pUnit;
	KPtr<Force_Unit>		m_pPUnit;

	MOVE_TYPE	m_MType;
	ACT_TYPE	m_AType;

	KVector		m_AIRot;
	KVector		m_AIPos;
	KVector		m_RenderRot;

private:
	void Init_Value();


	void Update_Death();
	void Update_Dir();
	void Update_RenCol();
	void Update_Move();
	void Update_Act();


	void Update_MIDLE();
	void Update_MOVE();

	void Update_AIDLE();

	void Update_BURROW();
	void Update_HIDE();
	void Update_UNBURROW();

	void Update_ATTACK01();
	void Update_ATTACK02();
	void Update_ATTACK03();
	void Update_DEATH();


	void Update_Terrain();

public:
	KVector& pos_player()
	{
		return m_AIPos;
	}

	KVector& rot_player()
	{
		return m_AIRot;
	}
	KVector& rot_render()
	{
		return m_RenderRot;
	}

	void Set_Render();
	void Add_Render(KPtr<Renderer_BonAni> _Other);
	void Del_Render(KPtr<Renderer_BonAni> _Other);

	bool Init(KPtr<Force_Unit>	_Unit);
	void Update();
};

