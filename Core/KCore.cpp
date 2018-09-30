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

// 시간으로 잡아서 윈도우를 계속 켜놓음
void KCore::Loop()
{
	MSG msg;
	while (true)
	{
		// GetMessage는 메시지가 발생하기 전까지 로직 중단시킨다.
		// 픽메시지는 메세지가 있을때만 처리하는 함수. -> 더 경제적
		// 이거 예기했던 부분
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