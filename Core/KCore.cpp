#include "KCore.h"

#include "KWindow.h"

#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Collision.h"
#include "SoundDevice.h"

#include "Core_Class.h"



KCore::CoreLauncher::CoreLauncher()
{

}

KCore::CoreLauncher::~CoreLauncher()
{

}

KCore::KCore()
{
}


KCore::~KCore()
{
}


void KCore::Init_Core(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, HWND _hWnd)
{
	//_CrtSetBreakAlloc(163450);
	KWindow::Init(_Hinst);
	if (nullptr == _hWnd)
	{
		if (nullptr == KWindow::Create_Window(_pMainWindowName))
		{
			return;
		}
	}
	else 
	{
		if (nullptr == KWindow::Create_Window(_pMainWindowName, _hWnd))
		{
			return;
		}
	}

	Core_Class::Main_Window(_pMainWindowName);

	SoundDevice::Init();
	PathManager::Init();
	PathManager::Create_ForderPath(L"Shader");
	PathManager::Create_ForderPath(L"Texture");
	PathManager::Create_ForderPath(L"Sound");

	TimeManager::Init();
	Collision::Init_Collision();
}


void KCore::Progress()
{
	TimeManager::Update();
	InputManager::Update();
	SoundDevice::Update();
	KWindow::Progress();
}

// �ð����� ��Ƽ� �����츦 ��� �ѳ���
void KCore::Loop()
{
	MSG msg;
	while (true)
	{
		// GetMessage�� �޽����� �߻��ϱ� ������ ���� �ߴܽ�Ų��.
		// �ȸ޽����� �޼����� �������� ó���ϴ� �Լ�. -> �� ������
		// �̰� �����ߴ� �κ�
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Progress();
		}

		if (0 == KWindow::WindowCount())
		{
			break;
		}
	}

}