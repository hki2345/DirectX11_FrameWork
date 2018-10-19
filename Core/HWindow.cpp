#include "HWindow.h"
#include "Stl_AID.h"

#include "KMacro.h"
#include "HResMgr.h"
#include "GameDebug.h"


////////////////////////////////// static

HINSTANCE KWindow::g_HInst = nullptr;


std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::WinStartIter;
std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::WinEndIter;

std::unordered_map<std::wstring, KPtr<KWindow>> KWindow::g_NWinMap;
std::unordered_map<HWND, KPtr<KWindow>> KWindow::g_HWinMap;

void KWindow::Init(HINSTANCE _HInst) 
{
	g_HInst = _HInst;
}


KPtr<KWindow> KWindow::CreateHWindow(const wchar_t* _Name, HWND _hWnd)
{
	// 스마트 포인터를 사용하기 때문에 이제 왠만
	KPtr<KWindow> Win = Map_Find<KPtr<KWindow>>(g_NWinMap, _Name);

	if (nullptr != Win)
	{
		return Win;
	}

	KWindow* pNewWindow = nullptr;

	if (nullptr == _hWnd)
	{
		pNewWindow = new KWindow(_Name);
	}
	else {
		pNewWindow = new KWindow(_Name, _hWnd);
	}

	if (nullptr == pNewWindow->m_HWnd)
	{
		delete pNewWindow;
		return nullptr;
	}

	pNewWindow->Set_Type();

	//if (true == pNewWindow->IsEqual<KWindow>())
	//{
	//	int a = 0;
	//}

	g_NWinMap.insert(std::unordered_map<std::wstring, KPtr<KWindow>>::value_type(_Name, pNewWindow));
	g_HWinMap.insert(std::unordered_map<HWND, KPtr<KWindow>>::value_type(pNewWindow->m_HWnd, pNewWindow));

	return pNewWindow;
}

KPtr<KWindow> KWindow::FindHWindow(const wchar_t* _Name) 
{
	return Map_Find<KPtr<KWindow>>(g_NWinMap, _Name);
}

void KWindow::EraseHWindow(const HWND _Handle)
{
	KPtr<KWindow> Win = Map_Find<KPtr<KWindow>>(g_HWinMap, _Handle);

	KASSERT(nullptr == Win);

	if (nullptr == Win)
	{
		return;
	}

	std::wstring Name = Win->Name();
	Map_Erase(g_HWinMap, _Handle);
	Map_Erase(g_NWinMap, Name.c_str());
}

void KWindow::Progress()
{
	WinStartIter = g_NWinMap.begin();
	WinEndIter = g_NWinMap.end();

	for (; WinStartIter != WinEndIter; ++WinStartIter)
	{
		WinStartIter->second->Update();
	}
}

// 모든 여러개 만들어진 윈도우가 공유하는 윈도우 프록시 이다.
LRESULT CALLBACK KWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		EraseHWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



////////////////////////////////// MEMBER

KWindow::KWindow(const wchar_t* _Name) : Begin(_Name), m_HWnd(nullptr), SceneMgr(this), m_Device(this), m_bFull(true)
{
	RegClass();
	if (FALSE == InitInst())
	{
		m_HWnd = nullptr;
		return;
	}

	m_Hdc = GetDC(m_HWnd);
}

KWindow::KWindow(const wchar_t* _Name, HWND _hWnd) : Begin(_Name), m_HWnd(nullptr), SceneMgr(this), m_Device(this), m_bFull(true)
{
	m_HWnd = _hWnd;

	RECT RC;
	GetClientRect(_hWnd, &RC);

	m_Width = RC.right;
	m_Height = RC.bottom;

	m_Hdc = GetDC(m_HWnd);
}


KWindow::~KWindow()
{
}

void KWindow::Show(int _ShowOption) {
	ShowWindow(m_HWnd, _ShowOption);
	UpdateWindow(m_HWnd);
}

void KWindow::FullScr_On() {
	m_bFull = false;
}

void KWindow::FullScr_Off() {
	m_bFull = true;
}

void KWindow::size(const size_t&_X, const size_t& _Y)
{
	m_Width = _X;
	m_Height = _Y;

	RECT Rc = { 0, 0, (long)_X, (long)_Y };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_HWnd, nullptr, 0, 0, Rc.right - Rc.left
		, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void KWindow::Update() 
{
	SceneMgr.Progress();

	if (true == m_Device.IsInit())
	{
		// m_Device.ClearTaget();
		SceneMgr.Render();
		// m_Device.Present();
	}

	// 충돌
	SceneMgr.AfterProgress();
	SceneMgr.Release();
}


ATOM KWindow::RegClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_HInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszMenuName = nullptr; //  MAKEINTRESOURCEW(IDC_AR14APICLIENT);
	wcex.lpszClassName = Name();
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}


BOOL KWindow::InitInst()
{
	// 테두리 없는 윈도우를 만들고 싶다면 WS_OVERLAPPEDWINDOW
	// 다른 걸로 넣어줘야 한다.

	m_HWnd = CreateWindowW(Name(), L"Title", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, g_HInst, nullptr);

	if (!m_HWnd)
	{
		return FALSE;
	}

	return TRUE;
}

bool KWindow::DeviceInit() 
{
	bool Return = m_Device.Init();

	// m_Device.DefaultDataInit();
	m_Device.DefaultDataInit3D();

	return Return;
}