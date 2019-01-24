#pragma once
#include <State.h>
class EUpdater : public State::State_Updater
{
public:
	EUpdater();
	~EUpdater();


public:
	virtual void  Start_State() override;
	virtual void  Update_State() override;
};

