#include "stdafx.h"
#include "ToolSceneUpdater.h"
#include <InputMgr.h>
#include <DebugManager.h>
#include <HThread.h>
#include <TimeMgr.h>


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

void ToolSceneUpdater::SceneUpdate()
{
	pColFi->m_Vec.m_Pos = KVector2{0.0f, 0.0f};
	Check = false;

	pColFi->m_Vec.m_Pos = Scene()->Camera()->ScreenToWorld(InputMgr::MousePos());

	if (InputMgr::IsDown(L"Q"))
	{
		HThread::StartThread<ToolSceneUpdater>(L"TestThread", &ToolSceneUpdater::Func, this);

		//if (true == m_bTestThread)
		//{
		//	HThread::PauseThread(L"TestThread");
		//	CheckNumber = Number;
		//	m_bTestThread = false;
		//}
		//else {
		//	HThread::ResumThread(L"TestThread");
		//	m_bTestThread = true;
		//}
	}

	if (InputMgr::IsDownStay(L"Up"))
	{
		Scene()->Camera()->Actor()->Trans()->LMove(KVector4::Up * CameraSpeed * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDownStay(L"Down"))
	{
		Scene()->Camera()->Actor()->Trans()->LMove(KVector4::Down * CameraSpeed * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDownStay(L"Left"))
	{
		Scene()->Camera()->Actor()->Trans()->LMove(KVector4::Left * CameraSpeed * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDownStay(L"Right"))
	{
		Scene()->Camera()->Actor()->Trans()->LMove(KVector4::Right * CameraSpeed * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDown(L"MouseButton"))
	{
		KPtr<HColCom> m_Col = Scene()->Col2DMgr.UpdateColCheck(0, pColFi);
		if (nullptr != m_Col)
		{
			m_SelectActor = m_Col->Actor();
		}
	}

	if (InputMgr::IsUp(L"MouseButton"))
	{
		m_SelectActor = nullptr;
	}

	if (nullptr != m_SelectActor)
	{
		KVector4 ObjPos = m_SelectActor->Trans()->LPos();
		m_SelectActor->Trans()->LPos({ pColFi->m_Vec.m_Pos.x, pColFi->m_Vec.m_Pos.y , ObjPos.z });
	}
}

unsigned int ToolSceneUpdater::Func(void* _Test)
{
	Number = 0;
	float Time = 0.0f;

	// 1. 항상 스택을 사용하면 안전하다.
	TimeMgr::GameTimer Timer;

	// 2. 터미네이트 쓰레드는 항상 위험한다.

	// KPtr<TimeMgr::GameTimer> ptr = TimeMgr::CreatTimer(L"TestTimer");

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

void ToolSceneUpdater::SceneStart()
{
	HThread::StartThread<ToolSceneUpdater>(L"TestThread", &ToolSceneUpdater::Func, this);
}

void ToolSceneUpdater::DebugRender() {

	wchar_t Arr[256];

	

	swprintf_s(Arr, L"ScreenPos : %f, %f", InputMgr::MousePos().x, InputMgr::MousePos().y);

	DebugManager::DrawFont(Arr, { 10.0f, 40.0f }, 20.0f);

	KVector2 MPos = Scene()->Camera()->ScreenToWorld(InputMgr::MousePos());

	swprintf_s(Arr, L"MWorldPos : %f, %f", MPos.x, MPos.y);
	DebugManager::DrawFont(Arr, { 10.0f, 60.0f }, 20.0f);

	if (false == Check)
	{
		DebugManager::DrawFont(L"안걸림", { 10.0f, 10.0f }, 40.0f);
	}
	else {
		DebugManager::DrawFont(L"걸려부렀어", { 10.0f, 10.0f }, 40.0f);
	}

	if (false == m_bTestThread)
	{
		DebugManager::DrawFont(L"멈춰", { 300.0f, 10.0f }, 40.0f);
	}
	else {
		DebugManager::DrawFont(L"안멈춰", { 300.0f, 10.0f }, 40.0f);
	}

	

	_itow_s(Number, Arr, 10);

	DebugManager::DrawFont(Arr, { 10.0f, 80.0f }, 50.0f);

	_itow_s(CheckNumber, Arr, 10);
	DebugManager::DrawFont(Arr, { 320.0f, 80.0f }, 50.0f);
}