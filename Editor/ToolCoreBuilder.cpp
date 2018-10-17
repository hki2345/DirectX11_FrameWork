#include "stdafx.h"
#include "ToolCoreBuilder.h"
#include <HVAR.h>
#include <HWindow.h>
#include <HResMgr.h>
#include <InputMgr.h>
#include <HSound.h>

ToolCoreBuilder::ToolCoreBuilder()
{
}


ToolCoreBuilder::~ToolCoreBuilder()
{
}

void ToolCoreBuilder::Build()
{
	HVAR::MainWindow().DeviceInit();

	GamePath::CreateRootPath(L"Data");
	GamePath::CreateRootPath(L"SpriteBack", L"Data\\Back\\Sprite");

	InputMgr::CreateKey(L"RMouseButton", VK_RBUTTON);
	InputMgr::CreateKey(L"MouseButton", VK_LBUTTON);
	InputMgr::CreateKey(L"Up", 'W');
	InputMgr::CreateKey(L"Left", 'A');
	InputMgr::CreateKey(L"Down", 'S');
	InputMgr::CreateKey(L"Right", 'D');
	InputMgr::CreateKey(L"Q", 'Q');

//	HResMgr<HSound>::Load(L"Sound", L"1-Up.wav");

}