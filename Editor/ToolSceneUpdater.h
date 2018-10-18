#pragma once
#include <HScene.h>

class ToolSceneUpdater : public HScene::HSceneUpdater
{
public:
	Base2DColFi* pColFi;
	bool Check;

	float CameraSpeed;

	KPtr<HActor> m_SelectActor;

	int Number;
	int CheckNumber;

	bool m_bTestThread;

public:
	// HSceneUpdater을(를) 통해 상속됨
	virtual void SceneUpdate() override;

	// HSceneUpdater을(를) 통해 상속됨
	virtual void SceneStart() override;

	virtual void DebugRender() override;

	unsigned int Func(void* _Test);

public:
	ToolSceneUpdater();
	~ToolSceneUpdater();
};

