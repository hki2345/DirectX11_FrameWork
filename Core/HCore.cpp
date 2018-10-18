#include "HCore.h"
#include "HWindow.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "HVAR.h"
#include "HResMgr.h"
#include "HColCom.h"
#include "HSoundDevice.h"
#include "HFBX.h"
/////////////////////////// Builder

HCore::HCoreBuilder::HCoreBuilder() 
{

}

HCore::HCoreBuilder::~HCoreBuilder() 
{

}

/////////////////////////// Core

HCore::HCore()
{
}


HCore::~HCore()
{
}

int HCore::Loop() 
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

		if (0 == HWindow::WindowCount())
		{
			break;
		}
	}

	return 1;
}

void HCore::Progress() 
{
	TimeMgr::Update();
	InputMgr::Update();
	HSoundDevice::Update();
	HWindow::Progress();
	// ���� Ÿ���� �̿��Ұ��̴�.
}

void HCore::CoreInit(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, HWND _hWnd) {
	HWindow::Init(_Hinst);
	if (nullptr == _hWnd)
	{
		if (nullptr == HWindow::CreateHWindow(_pMainWindowName))
		{
			return;
		}
	}
	else {
		if (nullptr == HWindow::CreateHWindow(_pMainWindowName, _hWnd))
		{
			return;
		}
	}

	HVAR::MainWindow(_pMainWindowName);
	HSoundDevice::Init();
	GamePath::Init();
	GamePath::CreateRootPath(L"Shader");
	GamePath::CreateRootPath(L"Texture");
	GamePath::CreateRootPath(L"Sound");
	TimeMgr::Init();
	HColCom::ColInit();
}