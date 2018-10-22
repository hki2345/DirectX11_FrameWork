#pragma once
#include <State.h>

class Edit_SUpdater : public State::State_Updater
{
public:
	Base2DColFi* pColFi;
	bool Check;

	float CameraSpeed;

	KPtr<TheOne> m_SelectActor;

	int Number;
	int CheckNumber;

	bool m_bTestThread;

public:
	// State_Updater을(를) 통해 상속됨
	virtual void Update_State() override;

	// State_Updater을(를) 통해 상속됨
	virtual void Start_State() override;

	virtual void DebugRender() override;

	unsigned int Func(void* _Test);

public:
	Edit_SUpdater();
	~Edit_SUpdater();
};

