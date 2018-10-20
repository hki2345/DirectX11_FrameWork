#include "stdafx.h"
#include "ToolCoreBuilder.h"
#include <Core_Class.h>
#include <KWindow.h>
#include <HResMgr.h>
#include <InputMgr.h>


#include <HImage.h>
#include <HSound.h>

ToolCoreBuilder::ToolCoreBuilder()
{
}


ToolCoreBuilder::~ToolCoreBuilder()
{
}

void ToolCoreBuilder::Build()
{

	PathManager::Create_ForderPath(L"Shader");
	PathManager::Create_ForderPath(L"Texture");
	PathManager::Create_ForderPath(L"Sound");
	PathManager::Create_ForderPath(L"Mesh");
	PathManager::Create_ForderPath(L"Data");
	PathManager::Create_ForderPath(L"SpriteBack", L"Data\\Back\\Sprite");

	// ResourceManager<HImage>::All_Load(L"Texture");
	// ResourceManager<HSound>::All_Load(L"Sound");
	
	Core_Class::MainWindow().Init_Device();

	InputMgr::CreateKey(L"RMouseButton", VK_RBUTTON);
	InputMgr::CreateKey(L"MouseButton", VK_LBUTTON);
	InputMgr::CreateKey(L"Up", 'W');
	InputMgr::CreateKey(L"Left", 'A');
	InputMgr::CreateKey(L"Down", 'S');
	InputMgr::CreateKey(L"Right", 'D');
	InputMgr::CreateKey(L"Q", 'Q');


}