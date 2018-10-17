#pragma once
#include <HScene.h>
#include <Windows.h>

class TestSceneUpdater : public HScene::HSceneUpdater
{
public:

public:
	TestSceneUpdater();
	~TestSceneUpdater();

	// HSceneUpdater을(를) 통해 상속됨
	virtual void SceneUpdate() override;

	// HSceneUpdater을(를) 통해 상속됨
	virtual void SceneStart() override;
};

