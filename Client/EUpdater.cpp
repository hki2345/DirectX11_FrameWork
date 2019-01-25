#include "EUpdater.h"
#include <Core_Class.h>
#include <InputManager.h>


EUpdater::EUpdater()
{
}


EUpdater::~EUpdater()
{
}

void  EUpdater::Start_State()
{

}

void  EUpdater::Update_State()
{
	if (true == KEY_DOWN(L"LB"))
	{
		Core_Class::MainSceneMgr().Change_State(L"Start");
	}
}