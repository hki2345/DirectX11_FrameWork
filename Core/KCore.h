#pragma once


#include <Windows.h>

#include <crtdbg.h>

#include "SmartPtr.h"

class KCore
{
public:
	class CoreLauncher : public SmartPtr
	{
	public:
		virtual void Build() = 0;

	protected:
		CoreLauncher();
		virtual ~CoreLauncher() = 0;
	};


private:
	friend int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);

public:
	template<typename T>
	static int Start(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, HWND _hWnd = nullptr)
	{
		new int;
		Init_Core(_Hinst, _pMainWindowName, _hWnd);

		T m_pBuilder = T();
		m_pBuilder.Build();

		return 1;
	}

private:
	static void Init_Core(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, HWND _hWnd = nullptr);
	static void Loop();

public:
	static void Progress();

public:
	KCore();
	~KCore();
};


#pragma region CORESTART
#define CORELAUNCHER(BUILDER, WINNAME) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); \
	KCore::Start<BUILDER>(hInstance, WINNAME); \
	KCore::Loop(); \
	return 0; \
}
#pragma endregion
