#pragma once
#include <State.h>

class TestSceneBuilder : public State::State_Builder
{

public:
	TestSceneBuilder();
	~TestSceneBuilder();

	// State_Builder을(를) 통해 상속됨
	virtual void Build_State() override;
};

