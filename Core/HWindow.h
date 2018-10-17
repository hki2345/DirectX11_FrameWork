#pragma once
#include "HSceneMgr.h"
#include "HDevice.h"
#include "HRenderMgr.h"

#include <Windows.h>
// #include "hpotr"
// 해시로 관리된다.
#include <unordered_map>


// "MainWindow"
// int HeshNumber = Hesh("MainWindow"); 24
// int HeshNumber = Hesh("SubWindow"); 85
// 배열로 관리할수 있다.

class HCore;
class HWindow : public HObjectBase
{
public:
	friend HCore;

private:
	static std::unordered_map<std::wstring, HPTR<HWindow>>::iterator WinStartIter;
	static std::unordered_map<std::wstring, HPTR<HWindow>>::iterator WinEndIter;

	static std::unordered_map<std::wstring, HPTR<HWindow>> g_NWinMap;
	static std::unordered_map<HWND, HPTR<HWindow>> g_HWinMap;

private:
	static HINSTANCE g_HInst;
	static void Init(HINSTANCE _HInst);

public:
	static HPTR<HWindow> CreateHWindow(const wchar_t* _Name, HWND _hWnd = nullptr);
	static HPTR<HWindow> FindHWindow(const wchar_t* _Name);
	static void EraseHWindow(const HWND _Handle);
	static void Progress();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static size_t WindowCount()
	{
		return g_NWinMap.size();
	}

	///////////////////////////////////////////// Member

private:
	HWND m_HWnd;
	HDC m_Hdc;
	size_t m_Width;
	size_t m_Height;
	bool m_bFull;

protected:
	HWindow(const wchar_t* _Name);
	HWindow(const wchar_t* _Name, HWND _hWnd);
	~HWindow();

public:

	bool IsFull() {
		return !m_bFull;
	}

	HWND WINHWND() {
		return m_HWnd;
	}

	HDC WINDC() {
		return m_Hdc;
	}

	UINT UWidth() {
		return (unsigned)m_Width;
	}

	UINT UHeight() {
		return (unsigned)m_Height;
	}

	size_t Width() {
		return m_Width;
	}

	size_t Height() {
		return m_Height;
	}

	float FWidth() {
		return (float)m_Width;
	}

	float FHeight() {
		return (float)m_Height;
	}

	HVEC2 Size() {
		return{ FWidth(), FHeight() };
	}

public:
	void Show(int _ShowOption = SW_SHOWDEFAULT);
	void Size(size_t _X, size_t _Y);
	void FullScreenOn();
	void FullScreenOff();

private:
	void Update();
	ATOM RegClass();
	BOOL InitInst();

public:
	HSceneMgr SceneMgr;

////////////////////////////////////////////////////////// 디바이스
private:
	HDevice m_Device;

public:
	HDevice& Device() { return m_Device; }

public:
	bool DeviceInit();

private:
	HRenderMgr m_RenderMgr;

};

