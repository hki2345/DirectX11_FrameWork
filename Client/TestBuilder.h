#pragma once
#include <State.h>


class TestBuilder : public State::State_Builder
{
public:
	TestBuilder();
	~TestBuilder();

public:
	void Build_State() override;
};

