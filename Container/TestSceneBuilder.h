#pragma once
#include <State.h>

class TestSceneBuilder : public State::State_Builder
{

public:
	TestSceneBuilder();
	~TestSceneBuilder();

	// State_Builder��(��) ���� ��ӵ�
	virtual void Build_State() override;
};

