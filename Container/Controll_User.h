#pragma once
#include <Component.h>
#include <list>

class Force_Unit;
class SC2_Camera;
class Renderer_Terrain;
class Renderer_BonAni;
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
	bool m_MirrorY;

	std::list<KPtr<Renderer_BonAni>> m_RList;
	std::list<KPtr<Renderer_BonAni>>::iterator m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator m_ERI;

	KPtr<Force_Unit>		m_pUnit;
	KPtr<SC2_Camera>		m_pCam;
	KPtr<Renderer_Terrain>	m_pTer;

	MOVE_TYPE	m_MType;
	ACT_TYPE	m_AType;

	KVector		m_PlayRot;
	KVector		m_PlayPos;
	KVector		m_RenderRot;

private:
	void Update_RenCol();
	void Update_Move();
	void Update_Act();
	void Update_Mouse();


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
	KVector& pos_player()
	{
		return m_PlayPos;
	}

	KVector& rot_player()
	{
		return m_PlayRot;
	}
	KVector& rot_render()
	{
		return m_RenderRot;
	}

	void Set_Render();
	void Add_Render(KPtr<Renderer_BonAni> _Other);
	void Del_Render(KPtr<Renderer_BonAni> _Other);

	bool Init(KPtr<Renderer_Terrain> _Terrain, KPtr<Force_Unit>	_Unit, KPtr<SC2_Camera> _Cam);
	void Update();
};

