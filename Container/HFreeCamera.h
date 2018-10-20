#pragma once
#include "HLogic.h"
#include <HCamera.h>


class HFreeCamera : public HLogic
{
private:
	KPtr<HCamera> m_Camera;
	KVector2 PrevMouse;
	KVector2 RotPos;
	float m_RotSpeed;
	float m_Speed;
	float m_Boost;

public:
	bool Init() override;
	void Update() override;
	void DebugRender() override;

public:
	HFreeCamera();
	~HFreeCamera();
};

