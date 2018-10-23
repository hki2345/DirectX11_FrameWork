#pragma once
#include "SomeOne.h"
#include <Camera.h>
#include <DebugManager.h>


class Camera_Free : public SomeOne
{
public:
	Camera_Free();
	~Camera_Free();

private:
	KPtr<Camera> m_Cam;
	KVector2 PrevMouse;
	KVector2 RotPos;
	float m_RotSpeed;
	float m_Speed;
	float m_Boost;

private:
	bool Update_ScrCheck();

public:
	bool Init() override;
	void Update() override;
};

