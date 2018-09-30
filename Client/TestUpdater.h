#pragma once
#include <State.h>

class TestUpdater : public State::State_Updater
{
public:
	TestUpdater();
	~TestUpdater();

public:
	virtual void  Start_State() override;
	virtual void  Update_State() override;

};

