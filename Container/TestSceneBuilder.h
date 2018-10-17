#pragma once
#include <HScene.h>

class TestSceneBuilder : public HScene::HSceneBuilder
{

public:
	TestSceneBuilder();
	~TestSceneBuilder();

	// HSceneBuilder을(를) 통해 상속됨
	virtual void SceneBuild() override;
};

