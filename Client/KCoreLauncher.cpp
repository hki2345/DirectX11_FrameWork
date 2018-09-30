#include "KCoreLauncher.h"
#include <memory>
#include <KWindow.h>
#include <InputManager.h>
#include <Core_Class.h>
#include <ResourceManager.h>
#include <KImage.h>
#include <KSound.h>
#include <KFont.h>
#include <Texture.h>
#include <DebugManager.h>

#include "TestBuilder.h"
#include "TestUpdater.h"

KCoreLauncher::KCoreLauncher()
{
}


KCoreLauncher::~KCoreLauncher()
{
}

void KCoreLauncher::Build()
{
	Core_Class::Main_Window().size(1280, 720);
	Core_Class::Main_Window().Show();
	Core_Class::Main_Window().Init_Device();

	PathManager::Create_ForderPath(L"Sprite_Data", L"Data\\Sprite");
	PathManager::Create_ForderPath(L"Sprite_Back", L"Data\\Back\\Sprite");
	PathManager::Create_ForderPath(L"KRigid2D_Data", L"Data\\KRigid2D");
	PathManager::Create_ForderPath(L"KRigid2D_Back", L"Data\\Back\\KRigid2D");
	PathManager::Create_ForderPath(L"Panza", L"Data\\Panza");
	PathManager::Create_ForderPath(L"State", L"Data\\State");
	PathManager::Create_ForderPath(L"User", L"Data\\User");

	ResourceManager<KImage>::All_Image_Load(L"Texture");
	ResourceManager<KSound>::All_Sound_Load(L"Sound"); 
	
	ResourceManager<KFont>().Create(L"µ¸¿ò", L"µ¸¿ò");

	InputManager::Create_Command(L"MOUSE_LB", VK_LBUTTON);
	InputManager::Create_Command(L"MOUSE_RB", VK_RBUTTON);
	InputManager::Create_Command(L"MOUSE_MB", VK_MBUTTON);
	InputManager::Create_Command(L"Up", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Down", 'S');
	InputManager::Create_Command(L"Right", 'D');

	Core_Class::Main_StateManager().create_state<TestBuilder, TestUpdater>(L"Test");


		
	// Ã¹¾À ¤·¤·
	Core_Class::Main_StateManager().change_state(L"Test");

#if _DEBUG
	DebugManager::Debug_On();
#else
	DebugManager::Debug_Off();
#endif // _DEBUG
}