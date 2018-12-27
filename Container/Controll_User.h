#pragma once
#include <Component.h>


class Force_Unit;
class SC2_Camera;
class Renderer_Terrain;
class Controll_User : public Component
{
public:
	Controll_User();
	~Controll_User();


public:
	enum MOVE_TYPE
	{
		MT_IDLE = 0,
		MT_MOVE,
		MT_RUN,
	};

	enum ACT_TYPE
	{
		AT_IDLE = 0,
		AT_ATTACK,
		AT_BOMB,
		AT_HEAL,
		AT_OPTI
	};

private:
	KPtr<Force_Unit>		m_pUnit;
	KPtr<SC2_Camera>			m_pCam;
	KPtr<Renderer_Terrain>	m_pTer;

	MOVE_TYPE	m_MType;
	ACT_TYPE	m_AType;

private:
	void Update_Move();
	void Update_Act();


	void Update_MIDLE();
	void Update_MOVE();
	void Update_RUN();

	void Update_AIDLE();
	void Update_ATTACK();
	void Update_BOMB();
	void Update_HEAL();
	void Update_OPTI();


	void Update_Terrain();

public:
	bool Init(KPtr<Renderer_Terrain> _Terrain, KPtr<Force_Unit>	_Unit, KPtr<SC2_Camera> _Cam);
	void Update();
};

