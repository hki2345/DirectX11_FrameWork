#include "stdafx.h"
#include "ToolCoreBuilder.h"
#include <Core_Class.h>
#include <KWindow.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <KFont.h>


#include <KImage.h>
#include <Sound.h>

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

	Core_Class::MainWindow().Init_Device();

	ResourceManager<KImage>::All_Load(L"Texture");
	ResourceManager<Sound>::All_Load(L"Sound");
	ResourceManager<KFont>::Create(L"태나무", L"태나무");
	

	InputManager::CreateKey(L"RMouseButton", VK_RBUTTON);
	InputManager::CreateKey(L"MouseButton", VK_LBUTTON);
	InputManager::CreateKey(L"Up", 'W');
	InputManager::CreateKey(L"Left", 'A');
	InputManager::CreateKey(L"Down", 'S');
	InputManager::CreateKey(L"Right", 'D');
	InputManager::CreateKey(L"Q", 'Q');


}