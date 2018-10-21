#include "stdafx.h"
#include "ToolSceneUpdater.h"
#include <InputManager.h>
#include <DebugManager.h>
#include <KThread.h>
#include <TimeManager.h>


ToolSceneUpdater::ToolSceneUpdater()
{
	Number = 0;
	m_bTestThread = true;
	pColFi = new Base2DColFi();
	pColFi->m_ColType = COLTYPE::CT_POINT2D;
	CameraSpeed = 100.0f;
	DebugOn();
}


ToolSceneUpdater::~ToolSceneUpdater()
{

	delete pColFi;
}

void ToolSceneUpdater::Update_State()
{
	pColFi->m_Vec.m_Pos = KVector2{0.0f, 0.0f};
	Check = false;

	pColFi->m_Vec.m_Pos = state()->Camera()->ScreenTo_World(InputManager::MousePos());

	if (InputManager::Down(L"Q"))
	{
		KThread::Start_Thread<ToolSceneUpdater>(L"TestThread", &ToolSceneUpdater::Func, this);

		//if (true == m_bTestThread)
		//{
		//	KThread::Pause_Thread(L"TestThread");
		//	CheckNumber = Number;
		//	m_bTestThread = false;
		//}
		//else {
		//	KThread::Recov_Thread(L"TestThread");
		//	m_bTestThread = true;
		//}
	}

	if (InputManager::Press(L"Up"))
	{
		state()->Camera()->one()->Trans()->Moving(KVector4::Up * CameraSpeed * TimeManager::DeltaTime());
	}

	if (InputManager::Press(L"Down"))
	{
		state()->Camera()->one()->Trans()->Moving(KVector4::Down * CameraSpeed * TimeManager::DeltaTime());
	}

	if (InputManager::Press(L"Left"))
	{
		state()->Camera()->one()->Trans()->Moving(KVector4::Left * CameraSpeed * TimeManager::DeltaTime());
	}

	if (InputManager::Press(L"Right"))
	{
		state()->Camera()->one()->Trans()->Moving(KVector4::Right * CameraSpeed * TimeManager::DeltaTime());
	}

	if (InputManager::Down(L"MouseButton"))
	{
		KPtr<HColCom> m_Col = state()->Col2DMgr.UpdateColCheck(0, pColFi);
		if (nullptr != m_Col)
		{
			m_SelectActor = m_Col->one();
		}
	}

	if (InputManager::Up(L"MouseButton"))
	{
		m_SelectActor = nullptr;
	}

	if (nullptr != m_SelectActor)
	{
		KVector4 ObjPos = m_SelectActor->Trans()->pos_local();
		m_SelectActor->Trans()->pos_local({ pColFi->m_Vec.m_Pos.x, pColFi->m_Vec.m_Pos.y , ObjPos.z });
	}
}

unsigned int ToolSceneUpdater::Func(void* _Test)
{
	Number = 0;
	float Time = 0.0f;

	// 1. 항상 스택을 사용하면 안전하다.
	TimeManager::GameTimer Timer;

	// 2. 터미네이트 쓰레드는 항상 위험한다.

	// KPtr<TimeManager::GameTimer> ptr = TimeManager::CreatTimer(L"TestTimer");

	while (true)
	{
		Time += Timer.Update();

		if (Time > 0.5f)
		{
			++Number;
			Time = 0.0f;
		}
	}

	return 0;
}

void ToolSceneUpdater::Start_State()
{
	KThread::Start_Thread<ToolSceneUpdater>(L"TestThread", &ToolSceneUpdater::Func, this);
}

void ToolSceneUpdater::DebugRender() {

	wchar_t Arr[256];

	

	swprintf_s(Arr, L"ScreenPos : %f, %f", InputManager::MousePos().x, InputManager::MousePos().y);

	DebugManager::Draw_Font(Arr, { 10.0f, 40.0f }, 20.0f);

	KVector2 MPos = state()->Camera()->ScreenTo_World(InputManager::MousePos());

	swprintf_s(Arr, L"MWorldPos : %f, %f", MPos.x, MPos.y);
	DebugManager::Draw_Font(Arr, { 10.0f, 60.0f }, 20.0f);

	if (false == Check)
	{
		DebugManager::Draw_Font(L"안걸림", { 10.0f, 10.0f }, 40.0f);
	}
	else {
		DebugManager::Draw_Font(L"걸려부렀어", { 10.0f, 10.0f }, 40.0f);
	}

	if (false == m_bTestThread)
	{
		DebugManager::Draw_Font(L"멈춰", { 300.0f, 10.0f }, 40.0f);
	}
	else {
		DebugManager::Draw_Font(L"안멈춰", { 300.0f, 10.0f }, 40.0f);
	}

	

	_itow_s(Number, Arr, 10);

	DebugManager::Draw_Font(Arr, { 10.0f, 80.0f }, 50.0f);

	_itow_s(CheckNumber, Arr, 10);
	DebugManager::Draw_Font(Arr, { 320.0f, 80.0f }, 50.0f);
}