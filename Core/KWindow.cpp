#include "KWindow.h"
#include "Stl_AID.h"

#include "KMacro.h"
#include "ResourceManager.h"
#include "InputManager.h"



/*************** Static ************/

HINSTANCE KWindow::g_HInst = nullptr;


std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::m_KSI;
std::unordered_map<std::wstring, KPtr<KWindow>>::iterator KWindow::m_KEI;

std::unordered_map<std::wstring, KPtr<KWindow>> KWindow::g_WinMap;



// 持失切拭辞 制亀酔 淫恵 依級戚 叔楳 吉陥.
KWindow::KWindow(const wchar_t* _Name, HWND _hWnd) : Begin(_Name), m_HWnd(nullptr), statemanager(this), m_Device(this), m_bFull(true)
{
	if (nullptr == _hWnd)
	{
		KRegisterClass();
		if (FALSE == Init_Instance())
		{
			BBY;
		}

		m_Hdc = GetDC(m_HWnd);
		return;
	}
	else
	{
		m_HWnd = _hWnd;

		RECT RC;
		GetClientRect(_hWnd, &RC);

		m_Width = RC.right;
		m_Height = RC.bottom;

		m_Hdc = GetDC(m_HWnd);
	}
}


KWindow::~KWindow()
{
}


/*************** Load_ByThread ************/


void KWindow::Init(HINSTANCE _HInst) 
{
	g_HInst = _HInst;
}


KPtr<KWindow> KWindow::Create_KWindow(const wchar_t* _Name, HWND _hWnd)
{
	// 什原闘 匂昔斗研 紫遂馬奄 凶庚拭 戚薦 陣幻
	KPtr<KWindow> Win = Map_Find<KPtr<KWindow>>(g_WinMap, _Name);

	if (nullptr != Win)
	{
		return Win;
	}

	KWindow* pNewWindow = nullptr;

	pNewWindow = new KWindow(_Name, _hWnd);
	
	if (nullptr == pNewWindow->m_HWnd)
	{
		delete pNewWindow;
		return nullptr;
	}

	pNewWindow->Set_Type();
	

	g_WinMap.insert(std::unordered_map<std::wstring, KPtr<KWindow>>::value_type(_Name, pNewWindow));

	return pNewWindow;
}

KPtr<KWindow> KWindow::Find_KWindow(const wchar_t* _Name) 
{
	return Map_Find<KPtr<KWindow>>(g_WinMap, _Name);
}

void KWindow::Erase_KWindow(const wchar_t* _Name)
{
	KPtr<KWindow> Win = Map_Find<KPtr<KWindow>>(g_WinMap, _Name);

	KASSERT(nullptr == Win);

	if (nullptr == Win)
	{
		return;
	}

	std::wstring Name = Win->name();
	Map_Erase(g_WinMap, Name.c_str());
}


void KWindow::Erase_KWindow(const HWND _Hwnd)
{
	m_KSI = g_WinMap.begin();
	m_KEI = g_WinMap.end();

	for (; m_KSI != m_KEI; ++m_KSI)
	{
		if (m_KSI->second->m_HWnd == _Hwnd)
		{
			Map_Erase(g_WinMap, m_KSI->first);
			return;
		}
	}

	// 食奄識 走酔走 公馬檎 斗像 -> 蒸陥澗 社軒艦汚
	BBY;
}

void KWindow::Progress()
{
	m_KSI = g_WinMap.begin();
	m_KEI = g_WinMap.end();

	for (; m_KSI != m_KEI; ++m_KSI)
	{
		m_KSI->second->Update();
	}
}

// 乞窮 食君鯵 幻級嬢遭 制亀酔亜 因政馬澗 制亀酔 覗系獣 戚陥.
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
	{
		Erase_KWindow(hWnd);
	}
	break;

	// 焼神 原酔什 蕃精 五獣走稽 閤澗陥 せせせせせせ
	case WM_MOUSEWHEEL:
	{
		InputManager::Receive_Wheel((int)HIWORD(wParam));
	}	
	break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
	statemanager.Progress();

	if (true == m_Device.IsInit())
	{
		// m_Device.Clear_Target();
		statemanager.Render();
		// m_Device.Present();
	}

	// 中宜
	statemanager.NextProgress();
	statemanager.Release();
}


ATOM KWindow::KRegisterClass()
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
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = name();
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}


BOOL KWindow::Init_Instance()
{
	// 砺砧軒 蒸澗 制亀酔研 幻級壱 粛陥檎 WS_OVERLAPPEDWINDOW
	// 陥献 杏稽 隔嬢操醤 廃陥.

	m_HWnd = CreateWindowW(name(), L"Title", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, g_HInst, nullptr);

	if (!m_HWnd)
	{
		return FALSE;
	}

	return TRUE;
}

bool KWindow::Init_Device() 
{
	bool Return = m_Device.Init();

	// m_Device.Init_BasicFigure2D();
	m_Device.Init_BasicFigure3D();

	return Return;
}