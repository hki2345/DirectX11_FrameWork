#include "TestSceneUpdater.h"
#include <HWindow.h>
#include <InputMgr.h>
#include <TimeMgr.h>
#include <Core_Class.h>

TestSceneUpdater::TestSceneUpdater()
{

}

TestSceneUpdater::~TestSceneUpdater()
{
}

void TestSceneUpdater::SceneUpdate()
{
	if (InputMgr::IsDown(L"E"))
	{
		Core_Class::MainSceneMgr().ChangeScene(L"Test2");
	}

	if (InputMgr::IsDown(L"Q"))
	{
		Core_Class::MainSceneMgr().ChangeScene(L"Test");
	}

}

void TestSceneUpdater::SceneStart()
{
}

