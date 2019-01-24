#include "KCoreLauncher.h"
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
#include <SoundPlayer.h>

#include "SBuilder.h"
#include "SUpdater.h"
#include "GBuilder.h"
#include "GUpdater.h"
#include "EBuilder.h"
#include "EUpdater.h"

KCoreLauncher::KCoreLauncher()
{
}


KCoreLauncher::~KCoreLauncher()
{
}

void KCoreLauncher::Build()
{
	Core_Class::MainWindow().full_size();
	Core_Class::MainWindow().Show();



	PathManager::Create_ForderPath(L"Shader");
	PathManager::Create_ForderPath(L"Texture");
	PathManager::Create_ForderPath(L"Sound");
	PathManager::Create_ForderPath(L"Mesh");
	PathManager::Create_ForderPath(L"Data");
	PathManager::Create_ForderPath(L"KM3", L"Data\\KM3");
	PathManager::Create_ForderPath(L"KCA", L"Data\\KCA");
	PathManager::Create_ForderPath(L"KUD", L"Data\\KUD");
	PathManager::Create_ForderPath(L"KTD", L"Data\\KTD");
	PathManager::Create_ForderPath(L"FORCE", L"Data\\Force");
	PathManager::Create_ForderPath(L"STATE", L"Data\\State");
	PathManager::Create_ForderPath(L"SpriteBack", L"Data\\Back\\Sprite");

	// �̷��� ��θ� ���� �� �ؾ���
	Core_Class::MainWindow().Init_Device();

	// ����̽� �ʱ�ȭ�� �ϰ� �ؾ���
	ResourceManager<KImage>::All_Load();
	ResourceManager<Sound>::All_Load();
	ResourceManager<KFont>::Create(L"Kostar", L"Kostar");


	InputManager::Create_Command(L"DEL", VK_DELETE);
	InputManager::Create_Command(L"ESC", VK_ESCAPE);
	InputManager::Create_Command(L"RB", VK_RBUTTON);
	InputManager::Create_Command(L"LB", VK_LBUTTON);
	InputManager::Create_Command(L"Up", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Down", 'S');
	InputManager::Create_Command(L"Right", 'D');
	InputManager::Create_Command(L"E", 'E');
	InputManager::Create_Command(L"F", 'F');
	InputManager::Create_Command(L"Q", 'Q');
	InputManager::Create_Command(L"MUNLOCK", VK_MENU);
	InputManager::Create_Command(L"DEBUG", VK_LSHIFT, 'B');
	InputManager::Create_Command(L"INGAME", VK_LSHIFT, 'P');



	Core_Class::MainSceneMgr().Create_State<SBuilder, SUpdater>(L"Start");
	Core_Class::MainSceneMgr().Change_State(L"Start");

	Core_Class::MainSceneMgr().Create_State<GBuilder, GUpdater>(L"InGame");
	Core_Class::MainSceneMgr().Create_State<EBuilder, EUpdater>(L"End");



	// ��Ƽ �ؽ���
	KPtr<Texture_Multi> MTex = ResourceManager<Texture_Multi>::Create(L"FB");
	MTex->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"Stone.jpg", L"StoneBump.jpg");
	KPtr<Texture_Multi> MTex2 = ResourceManager<Texture_Multi>::Create(L"FC");
	MTex2->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"Lava.jpg", L"LavaBump.jpg");



#if _DEBUG
	DebugManager::Debug_On();
#else
	DebugManager::Debug_Off();
#endif // _DEBUG
}