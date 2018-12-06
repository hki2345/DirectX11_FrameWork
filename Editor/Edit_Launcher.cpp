#include "stdafx.h"
#include "Edit_Launcher.h"
#include <Core_Class.h>
#include <KWindow.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <KFont.h>


#include <KImage.h>
#include <Sound.h>

#include <KThread.h>
#include <Renderer_BonAni.h>
#include <Con_Class.h>

Edit_Launcher::Edit_Launcher()
{
}


Edit_Launcher::~Edit_Launcher()
{
}

void Edit_Launcher::Build()
{
	PathManager::Create_ForderPath(L"Shader");
	PathManager::Create_ForderPath(L"Texture");
	PathManager::Create_ForderPath(L"Sound");
	PathManager::Create_ForderPath(L"Mesh");
	PathManager::Create_ForderPath(L"Data");
	PathManager::Create_ForderPath(L"KM3", L"Data\\KM3");
	PathManager::Create_ForderPath(L"KCA", L"Data\\KCA");
	PathManager::Create_ForderPath(L"SpriteBack", L"Data\\Back\\Sprite");

	// 이렇게 경로를 잡은 뒤 해야함
	Core_Class::MainWindow().Init_Device();

	// 디바이스 초기화를 하고 해야함
	ResourceManager<KImage>::All_Load();
	ResourceManager<Sound>::All_Load();
	ResourceManager<KFont>::Create(L"태나무", L"태나무");
	

	InputManager::Create_Command(L"RMouseButton", VK_RBUTTON);
	InputManager::Create_Command(L"MouseButton", VK_LBUTTON);
	InputManager::Create_Command(L"Up", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Down", 'S');
	InputManager::Create_Command(L"Right", 'D');
	InputManager::Create_Command(L"Q", 'Q');
}