#pragma once
#include <HScene.h>

class TestSceneBuilder : public HScene::HSceneBuilder
{

public:
	TestSceneBuilder();
	~TestSceneBuilder();

	// HSceneBuilder��(��) ���� ��ӵ�
	virtual void SceneBuild() override;
};

