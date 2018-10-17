#pragma once
#include <Windows.h>
#include <crtdbg.h>

#include "RefBase.h"
#include "HMACRO.h"

// 다중상속
// 모호함을 최대한 없애고 이용할수 있는 방법
class HCore
{
public: // 딴애들이 이거 상속은 받아야 하잖아.
	// 빌더패턴은 객체와 동작을 클래스를 조립하듯이 완성하는 패턴
	// 빌더라는 클래스를 만들어서 
	class HCoreBuilder : public HRefBase
	{
	public:
		virtual void Build() = 0;

	protected:
		HCoreBuilder();
		virtual ~HCoreBuilder() = 0;
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
		CoreInit(_Hinst, _pMainWindowName, _hWnd);

		T m_pBuilder = T();
		m_pBuilder.Build();


		return 1;
	}


private:
	static void CoreInit(HINSTANCE _Hinst, const wchar_t* _pMainWindowName, HWND _hWnd = nullptr);
	static int Loop();

public:
	static void Progress();

private:
	HCore();
	~HCore();
};

#pragma region CORESTMACRO
#define CORESTART(BUILDER, WINNAME) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); \
	HCore::Start<BUILDER>(hInstance, WINNAME); \
	HCore::Loop(); \
	return 0; \
}
#pragma endregion