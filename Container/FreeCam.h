#pragma once
#include <Some_One.h>
#include <Camera.h>

class FreeCam : public Some_One
{
public:
	FreeCam();
	~FreeCam();

private:
	KPtr<Camera> m_Cam;
	KVector2 m_PrevMouse;
	KVector2 m_CurRot;
	float m_RotateSpd;
	float m_Speed;

	float m_Boost;

private:
	bool Update_ScrCheck();

public:
	bool Init() override;
	void Update() override;
};

