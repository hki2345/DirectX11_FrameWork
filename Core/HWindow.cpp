#include "HWindow.h"
#include "StlHelperFunc.h"

#include "HMACRO.h"
#include "HResMgr.h"
#include "GameDebug.h"


////////////////////////////////// static

HINSTANCE HWindow::g_HInst = nullptr;


std::unordered_map<std::wstring, HPTR<HWindow>>::iterator HWindow::WinStartIter;
std::unordered_map<std::wstring, HPTR<HWindow>>::iterator HWindow::WinEndIter;

std::unordered_map<std::wstring, HPTR<HWindow>> HWindow::g_NWinMap;
std::unordered_map<HWND, HPTR<HWindow>> HWindow::g_HWinMap;

void HWindow::Init(HINSTANCE _HInst) 
{
	g_HInst = _HInst;
}


HPTR<HWindow> HWindow::CreateHWindow(const wchar_t* _Name, HWND _hWnd)
{
	// 스마트 포인터를 사용하기 때문에 이제 왠만
	HPTR<HWindow> Win = MapFind<HPTR<HWindow>>(g_NWinMap, _Name);

	if (nullptr != Win)
	{
		return Win;
	}

	HWindow* pNewWindow = nullptr;

	if (nullptr == _hWnd)
	{
		pNewWindow = new HWindow(_Name);
	}
	else {
		pNewWindow = new HWindow(_Name, _hWnd);
	}

	if (nullptr == pNewWindow->m_HWnd)
	{
		delete pNewWindow;
		return nullptr;
	}

	pNewWindow->TypeSetting();

	//if (true == pNewWindow->IsEqual<HWindow>())
	//{
	//	int a = 0;
	//}

	g_NWinMap.insert(std::unordered_map<std::wstring, HPTR<HWindow>>::value_type(_Name, pNewWindow));
	g_HWinMap.insert(std::unordered_map<HWND, HPTR<HWindow>>::value_type(pNewWindow->m_HWnd, pNewWindow));

	return pNewWindow;
}

HPTR<HWindow> HWindow::FindHWindow(const wchar_t* _Name) 
{
	return MapFind<HPTR<HWindow>>(g_NWinMap, _Name);
}

void HWindow::EraseHWindow(const HWND _Handle)
{
	HPTR<HWindow> Win = MapFind<HPTR<HWindow>>(g_HWinMap, _Handle);

	TASSERT(nullptr == Win);

	if (nullptr == Win)
	{
		return;
	}

	std::wstring Name = Win->Name();
	StlErase(g_HWinMap, _Handle);
	StlErase(g_NWinMap, Name.c_str());
}

void HWindow::Progress()
{
	WinStartIter = g_NWinMap.begin();
	WinEndIter = g_NWinMap.end();

	for (; WinStartIter != WinEndIter; ++WinStartIter)
	{
		WinStartIter->second->Update();
	}
}

// 모든 여러개 만들어진 윈도우가 공유하는 윈도우 프록시 이다.
LRESULT CALLBACK HWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

HWindow::HWindow(const wchar_t* _Name) : HObjectBase(_Name), m_HWnd(nullptr), SceneMgr(this), m_Device(this), m_bFull(true)
{
	RegClass();
	if (FALSE == InitInst())
	{
		m_HWnd = nullptr;
		return;
	}

	m_Hdc = GetDC(m_HWnd);
}

HWindow::HWindow(const wchar_t* _Name, HWND _hWnd) : HObjectBase(_Name), m_HWnd(nullptr), SceneMgr(this), m_Device(this), m_bFull(true)
{
	m_HWnd = _hWnd;

	RECT RC;
	GetClientRect(_hWnd, &RC);

	m_Width = RC.right;
	m_Height = RC.bottom;

	m_Hdc = GetDC(m_HWnd);
}


HWindow::~HWindow()
{
}

void HWindow::Show(int _ShowOption) {
	ShowWindow(m_HWnd, _ShowOption);
	UpdateWindow(m_HWnd);
}

void HWindow::FullScreenOn() {
	m_bFull = false;
}

void HWindow::FullScreenOff() {
	m_bFull = true;
}

void HWindow::Size(size_t _X, size_t _Y) 
{
	m_Width = _X;
	m_Height = _Y;

	RECT Rc = { 0, 0, (long)_X, (long)_Y };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_HWnd, nullptr, 0, 0, Rc.right - Rc.left
		, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void HWindow::Update() 
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


ATOM HWindow::RegClass()
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


BOOL HWindow::InitInst()
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

bool HWindow::DeviceInit() 
{
	bool Return = m_Device.Init();

	// m_Device.DefaultDataInit();
	m_Device.DefaultDataInit3D();

	return Return;
}