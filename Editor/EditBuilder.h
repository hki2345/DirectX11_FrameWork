#pragma once
#include <State.h>


class EditBuilder : public State::State_Builder
{
public:
	EditBuilder();
	~EditBuilder();


	// State_Builder을(를) 통해 상속됨
	virtual void Build_State() override {};
};

