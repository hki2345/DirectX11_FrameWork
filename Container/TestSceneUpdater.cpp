#include "TestSceneUpdater.h"
#include <KWindow.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <Core_Class.h>

TestSceneUpdater::TestSceneUpdater()
{

}

TestSceneUpdater::~TestSceneUpdater()
{
}

void TestSceneUpdater::Update_State()
{
	if (InputManager::Down(L"E"))
	{
		Core_Class::MainSceneMgr().Change_State(L"Test2");
	}

	if (InputManager::Down(L"Q"))
	{
		Core_Class::MainSceneMgr().Change_State(L"Test");
	}

}

void TestSceneUpdater::Start_State()
{
}

