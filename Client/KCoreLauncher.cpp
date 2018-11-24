#include "KCoreLauncher.h"
#include <KWindow.h>
#include <InputManager.h>
#include <Core_Class.h>
#include <ResourceManager.h>


#include <KImage.h>
#include <Sound.h>
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
	PathManager::Create_ForderPath(L"Shader");
	PathManager::Create_ForderPath(L"Texture");
	PathManager::Create_ForderPath(L"Sound");
	PathManager::Create_ForderPath(L"Mesh");
	PathManager::Create_ForderPath(L"Data");

	Core_Class::MainWindow().size(1280, 720);
	Core_Class::MainWindow().Show();

	// 패스를 만드로고 여 안에서 쉐이더를 만들겠지
	Core_Class::MainWindow().Init_Device();


	ResourceManager<KImage>::All_Load(L"Texture");
	ResourceManager<Sound>::All_Load(L"Sound");	
	ResourceManager<KFont>::Create(L"돋움", L"돋움");

	InputManager::Create_Command(L"MOUSE_LB", VK_LBUTTON);
	InputManager::Create_Command(L"MOUSE_RB", VK_RBUTTON);
	InputManager::Create_Command(L"MOUSE_MB", VK_MBUTTON);
	InputManager::Create_Command(L"Up", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Down", 'S');
	InputManager::Create_Command(L"Right", 'D');

	Core_Class::MainSceneMgr().Create_State<TestBuilder, TestUpdater>(L"Test");


		
	// 첫씬 ㅇㅇ
	Core_Class::MainSceneMgr().Change_State(L"Test");

#if _DEBUG
	DebugManager::Debug_On();
#else
	DebugManager::Debug_Off();
#endif // _DEBUG
}