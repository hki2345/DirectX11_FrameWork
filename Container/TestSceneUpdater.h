#pragma once
#include <HScene.h>
#include <Windows.h>

class TestSceneUpdater : public HScene::HSceneUpdater
{
public:

public:
	TestSceneUpdater();
	~TestSceneUpdater();

	// HSceneUpdater��(��) ���� ��ӵ�
	virtual void SceneUpdate() override;

	// HSceneUpdater��(��) ���� ��ӵ�
	virtual void SceneStart() override;
};

