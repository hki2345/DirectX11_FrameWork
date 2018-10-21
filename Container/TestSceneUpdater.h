#pragma once
#include <State.h>
#include <Windows.h>

class TestSceneUpdater : public State::State_Updater
{
public:

public:
	TestSceneUpdater();
	~TestSceneUpdater();

	// State_Updater을(를) 통해 상속됨
	virtual void Update_State() override;

	// State_Updater을(를) 통해 상속됨
	virtual void Start_State() override;
};

