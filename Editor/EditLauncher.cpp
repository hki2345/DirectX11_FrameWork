#include "stdafx.h"
#include "EditLauncher.h"
#include <Core_Class.h>
#include <KWindow.h>

#include <DebugManager.h>
#include <ResourceManager.h>
#include <Sprite_Changer.h>
#include <Texture.h>
#include <InputManager.h>
#include <KFont.h>
#include <KSound.h>


#include "Edit_Class.h"

EditLauncher::EditLauncher()
{
}


EditLauncher::~EditLauncher()
{
}


void EditLauncher::Build()
{
	Core_Class::Main_Window().Init_Device();

	InputManager::Create_Command(L"MOUSE_MB", VK_MBUTTON);
	InputManager::Create_Command(L"MOUSE_LB", VK_LBUTTON);
	InputManager::Create_Command(L"MOUSE_RB", VK_RBUTTON);


	InputManager::Create_Command(L"Up", VK_SPACE);
	InputManager::Create_Command(L"Down", VK_LCONTROL);
	InputManager::Create_Command(L"Forward", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Back", 'S');
	InputManager::Create_Command(L"Right", 'D');
	InputManager::Create_Command(L"Cam Rotate", VK_LBUTTON);

	InputManager::Create_Command(L"LeftRot", 'Q');
	InputManager::Create_Command(L"RightRot", 'E');

	InputManager::Create_Command(L"Quit", 'X');

	ResourceManager<KFont>().Create(L"µ¸¿ò", L"µ¸¿ò");

	PathManager::Create_ForderPath(L"Sprite_Data", L"Data\\Sprite");
	PathManager::Create_ForderPath(L"Sprite_Back", L"Data\\Back\\Sprite");
	PathManager::Create_ForderPath(L"KRigid2D_Data", L"Data\\KRigid2D");
	PathManager::Create_ForderPath(L"KRigid2D_Back", L"Data\\Back\\KRigid2D");
	PathManager::Create_ForderPath(L"Panza", L"Data\\Panza");
	PathManager::Create_ForderPath(L"State", L"Data\\State");
	PathManager::Create_ForderPath(L"User", L"Data\\User");
	
	 ResourceManager<KImage>::All_Load(L"Texture");
	 ResourceManager<KSound>::All_Load(L"Sound");

	DebugManager::Log_Color(KColor::White);
}