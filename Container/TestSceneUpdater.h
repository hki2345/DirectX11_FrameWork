#pragma once
#include <State.h>
#include <Windows.h>

class TestSceneUpdater : public State::State_Updater
{
public:

public:
	TestSceneUpdater();
	~TestSceneUpdater();

	// State_Updater��(��) ���� ��ӵ�
	virtual void Update_State() override;

	// State_Updater��(��) ���� ��ӵ�
	virtual void Start_State() override;
};

