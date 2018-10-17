#pragma once
#include "HLogic.h"
#include <HCamera.h>


class HFreeCamera : public HLogic
{
private:
	HPTR<HCamera> m_Camera;
	HVEC2 PrevMouse;
	HVEC2 RotPos;
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

