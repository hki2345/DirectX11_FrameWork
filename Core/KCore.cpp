#include "KCore.h"
#include "KWindow.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Core_Class.h"
#include "ResourceManager.h"
#include "HColCom.h"
#include "SoundDevice.h"

/////////////////////////// Launcher

KCore::Launcher::Launcher() 
{

}

KCore::Launcher::~Launcher() 
{

}

/////////////////////////// Core

KCore::KCore()
{
}


KCore::~KCore()
{
}

int KCore::Loop() 
{
	MSG msg;
	while (true)
	{
		// GetMessage�� �޽����� �߻��ϱ� ������ ���� �ߴܽ�Ų��.
		// �ȸ޽����� �޼����� �������� ó���ϴ� �Լ�.
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

	return 1;
}

void KCore::Progress() 
{
	TimeManager::Update();
	InputManager::Update();
	SoundDevice::Update();
	KWindow::Progress();
	// ���� Ÿ���� �̿��Ұ��̴�.
}

void KCore::Init_Core(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, HWND _hWnd) {

	// ������ �ʱ�ȭ
	KWindow::Init(_Hinst);
	if (nullptr == _hWnd)
	{
		if (nullptr == KWindow::CreateHWindow(_pMainWindowName))
		{
			return;
		}
	}
	else {
		if (nullptr == KWindow::CreateHWindow(_pMainWindowName, _hWnd))
		{
			return;
		}
	}
	

	Core_Class::MainWindow(_pMainWindowName);
	SoundDevice::Init();
	PathManager::Init();
	TimeManager::Init();
	HColCom::ColInit();
}