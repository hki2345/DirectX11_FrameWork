#include "KWindow.h"
#include "Stl_Assistor.h"
#include "ResourceManager.h"

#include "KMacro.h"
#include "DebugManager.h"

HINSTANCE KWindow::g_HInst = nullptr;

std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::m_StartIter;
std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::m_EndIter;

std::unordered_map<std::wstring, KPtr<KWindow>> KWindow::g_NWinMap;
std::unordered_map<HWND, KPtr<KWindow>> KWindow::g_HWinMap;



/*************** Func ************/

KWindow::KWindow(const wchar_t* _Name) :
	Begin(_Name), m_HWnd(nullptr), this_StateManager(this), m_Device(this), m_bFull(true)
{
	Register_Class();

	// 만들지 못했을 시 윈도우를 가리키는 포인터를 널로 만듬 ㅇㅇ
	if (FALSE == Init_Inst())
	{
		m_HWnd = nullptr;
		return;
	}

	m_Hdc = GetDC(m_HWnd);
}


KWindow::KWindow(const wchar_t* _Name, HWND _hWnd) :
	Begin(_Name), m_HWnd(nullptr), this_StateManager(this), m_Device(this), m_bFull(true)
{
	m_HWnd = _hWnd;

	RECT TRC;
	// 현재 활성중인 윈도우 창의 크기를 가져옴
	GetClientRect(_hWnd, &TRC);

	m_Width = TRC.right;
	m_Height = TRC.bottom;

	m_Hdc = GetDC(m_HWnd);
}


KWindow::~KWindow()
{
}



void KWindow::Init(HINSTANCE _HInst)
{
	g_HInst = _HInst;
}





KPtr<KWindow> KWindow::Create_Window(const wchar_t* _Name, HWND _hWnd)
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
	else
	{
		pNewWindow = new KWindow(_Name, _hWnd);
	}


	if (nullptr == pNewWindow->m_HWnd)
	{
		delete pNewWindow;
		return nullptr;
	}

	g_NWinMap.insert(std::unordered_map<std::wstring, KPtr<KWindow>>::value_type(_Name, pNewWindow));
	g_HWinMap.insert(std::unordered_map<HWND, KPtr<KWindow>>::value_type(pNewWindow->m_HWnd, pNewWindow));

	return pNewWindow;
}


KPtr<KWindow> KWindow::Find_Window(const wchar_t* _Name)
{
	// 스마트 포인터를 사용하기 때문에 이제 왠만
	return Map_Find<KPtr<KWindow>>(g_NWinMap, _Name);
}


void KWindow::Erase_Window(const HWND _Handle)
{
	KPtr<KWindow> Win = Map_Find<KPtr<KWindow>>(g_HWinMap, _Handle);

	KASSERT(nullptr == Win);

	if (nullptr == Win)
	{
		return;
	}

	std::wstring name = Win->name();
	Map_Erase(g_HWinMap, _Handle);
	Map_Erase(g_NWinMap, name.c_str());
}

void KWindow::Progress()
{
	m_StartIter = g_NWinMap.begin();
	m_EndIter = g_NWinMap.end();

	for (; m_StartIter != m_EndIter; m_StartIter++)
	{
		m_StartIter->second->Update();
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
		Erase_Window(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



/********************** Window ****************************/

void KWindow::Show(int _ShowOption) {
	ShowWindow(m_HWnd, _ShowOption);
	UpdateWindow(m_HWnd);
}

void KWindow::size(size_t _X, size_t _Y)
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
	this_StateManager.Progress();

	if (true == m_Device.Is_Init())
	{
		m_Device.Clear_Target();


		this_StateManager.Render();
		m_Device.Present();
	}

	this_StateManager.Next_Progress();
	this_StateManager.Release();
}


ATOM KWindow::Register_Class()
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
	wcex.lpszClassName = name();
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}


BOOL KWindow::Init_Inst()
{
	// 테두리 없는 윈도우를 만들고 싶다면 WS_OVERLAPPEDWINDOW
	// 다른 걸로 넣어줘야 한다.

	m_HWnd = CreateWindowW(name(), L"DirectX 3D", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, g_HInst, nullptr);
	if (!m_HWnd)
	{
		return FALSE;
	}

	return TRUE;
}

bool KWindow::Init_Device()
{
	bool _init = m_Device.Init();
	// m_Device.Init_DefaultData();
	m_Device.Init_DefaultData_3D();
	return _init;
}