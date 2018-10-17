#include "TestSceneUpdater.h"
#include <HWindow.h>
#include <InputMgr.h>
#include <TimeMgr.h>
#include <HVAR.h>

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
		HVAR::MainSceneMgr().ChangeScene(L"Test2");
	}

	if (InputMgr::IsDown(L"Q"))
	{
		HVAR::MainSceneMgr().ChangeScene(L"Test");
	}

}

void TestSceneUpdater::SceneStart()
{
}

