#pragma once
#include <Component.h>
#include "DXContainer.h"


class Force_Unit;
class Renderer_Terrain;
class Renderer_BonAni;
class Controll_Medivac : public Component
{
public:
	Controll_Medivac();
	~Controll_Medivac();


public:
	enum MOVE_TYPE
	{
		MT_IDLE = 0,
		MT_LAND,
	};

private:
	bool m_MirrorY;

	KPtr<Renderer_BonAni> m_Render;
	KPtr<Force_Unit>		m_pUnit;

	MOVE_TYPE	m_MType;

	KVector		m_TPos;
	KVector		m_PlayRot;
	KVector		m_PlayPos;
	KVector		m_RenderRot;

	float m_UTime;

private:
	void Update_MIDLE();
	void Update_LAND();


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

	bool Init(const KVector& _InitPos, KPtr<Renderer_Terrain> _Ter);
	void Update();
};

