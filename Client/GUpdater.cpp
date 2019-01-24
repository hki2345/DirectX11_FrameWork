#include "GUpdater.h"

#include <InputManager.h>
#include <Core_Class.h>


GUpdater::GUpdater()
{
}


GUpdater::~GUpdater()
{
}

void  GUpdater::Start_State()
{

}

void  GUpdater::Update_State()
{
	if (true == KEY_DOWN(L"LB"))
	{
		Core_Class::MainSceneMgr().Change_State(L"Start");
	}
}