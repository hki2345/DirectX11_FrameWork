#pragma once
#include <State.h>
class GUpdater : public State::State_Updater
{
public:
	GUpdater();
	~GUpdater();


public:
	virtual void  Start_State() override;
	virtual void  Update_State() override;

};

