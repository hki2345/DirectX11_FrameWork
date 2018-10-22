#include "stdafx.h"
#include "Edit_Launcher.h"
#include <Core_Class.h>
#include <KWindow.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <KFont.h>


#include <KImage.h>
#include <Sound.h>

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
	PathManager::Create_ForderPath(L"SpriteBack", L"Data\\Back\\Sprite");

	// �̷��� ��θ� ���� �� �ؾ���
	Core_Class::MainWindow().Init_Device();

	// ����̽� �ʱ�ȭ�� �ϰ� �ؾ���
	ResourceManager<KImage>::All_Load(L"Texture");
	ResourceManager<Sound>::All_Load(L"Sound");
	ResourceManager<KFont>::Create(L"�³���", L"�³���");
	

	InputManager::Create_Command(L"RMouseButton", VK_RBUTTON);
	InputManager::Create_Command(L"MouseButton", VK_LBUTTON);
	InputManager::Create_Command(L"Up", 'W');
	InputManager::Create_Command(L"Left", 'A');
	InputManager::Create_Command(L"Down", 'S');
	InputManager::Create_Command(L"Right", 'D');
	InputManager::Create_Command(L"Q", 'Q');


}