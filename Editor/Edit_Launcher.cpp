#include "stdafx.h"
#include "Edit_Launcher.h"
#include <Core_Class.h>
#include <KWindow.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <DebugManager.h>

#include <KFont.h>


#include <KImage.h>
#include <Texture_Multi.h>
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
	PathManager::Create_ForderPath(L"KUD", L"Data\\KUD");
	PathManager::Create_ForderPath(L"KTD", L"Data\\KTD");
	PathManager::Create_ForderPath(L"SpriteBack", L"Data\\Back\\Sprite");

	// 이렇게 경로를 잡은 뒤 해야함
	Core_Class::MainWindow().Init_Device();

	// 디바이스 초기화를 하고 해야함
	ResourceManager<KImage>::All_Load();
	ResourceManager<Sound>::All_Load();
	ResourceManager<KFont>::Create(L"태나무", L"태나무");
	

	InputManager::Create_Command(L"RB", VK_RBUTTON);
	InputManager::Create_Command(L"LB", VK_LBUTTON);
	InputManager::Create_Command(L"Up", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Down", 'S');
	InputManager::Create_Command(L"Right", 'D');
	InputManager::Create_Command(L"Q", 'Q');
	InputManager::Create_Command(L"DEBUG", VK_LSHIFT, 'B');



	// 멀티 텍스쳐
	KPtr<Texture_Multi> MTex = ResourceManager<Texture_Multi>::Create(L"FB");
	MTex->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"Stone.jpg", L"StoneBump.jpg");


	KPtr<Texture_Multi> MTex2 = ResourceManager<Texture_Multi>::Create(L"FC");
	MTex2->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"Lava.jpg", L"LavaBump.jpg");

	DebugManager::Debug_On();
}