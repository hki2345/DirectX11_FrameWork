#pragma once
#include "SomeOne.h"
#include <Camera.h>


class HFreeCamera : public SomeOne
{
private:
	KPtr<Camera> m_Camera;
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

