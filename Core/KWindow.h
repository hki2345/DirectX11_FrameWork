#pragma once
#include "StateManager.h"
#include "RenderManager.h"
#include "KDevice.h"

#include <Windows.h>
#include <unordered_map>

// 윈도우 창을 여러개 띄울 수 있게하며, 그 창을 해쉬로 관리한다.
class KWindow : public Begin
{
public:
	friend class KCore;

private:
	static std::unordered_map<std::wstring, KPtr<KWindow>>::iterator m_StartIter;
	static std::unordered_map<std::wstring, KPtr<KWindow>>::iterator m_EndIter;

	static std::unordered_map<std::wstring, KPtr<KWindow>> g_NWinMap;
	static std::unordered_map<HWND, KPtr<KWindow>> g_HWinMap;

private:
	static HINSTANCE g_HInst;
	static void Init(HINSTANCE _HInst);


	

	
public:
	// 드디어 크레이트 윈도우 - 어디서든 윈도우를 만들고 지울 수 있겠다.
	static KPtr<KWindow> Create_Window(const wchar_t* _Name, HWND _hWnd = nullptr);
	static KPtr<KWindow> Find_Window(const wchar_t* _Name);
	static void Erase_Window(const HWND _Handle);
	static void Progress();

private:
	// 윈도우가 만들어지면 모두 이 프록시를 실행하겠다.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


public:
	// 카운트 세는거
	static size_t WindowCount()
	{
		return g_NWinMap.size();
	}

	///////////////////////////////////////////// Member

private:
	// 현재 윈도우가 가진 HWND와 HDC
	HWND m_HWnd;
	HDC m_Hdc;
	size_t m_Width;
	size_t m_Height;
	bool m_bFull;

public:
	HWND KWnd() { return m_HWnd; }
	HDC KHdc()	{ return m_Hdc; }

	// 다이렉트 11에서는 Full모드가 false이고 창모드가 true이다. 따라서 한번 바뀌어야 함
	bool		Is_Full()	{ return !m_bFull; }

	UINT		widthu() { return (unsigned)m_Width; }
	UINT		heigthu() { return (unsigned)m_Height; }
	size_t&		width()		{ return m_Width; }
	size_t&		heigth()	{ return m_Height; }
	float		widthf()	{ return (float)m_Width; }
	float		heigthf()	{ return (float)m_Height; }
	
	void Show(int _ShowOption = SW_SHOWDEFAULT);
	void size(size_t _X, size_t _Y);
	KVector2 size() const { return KVector2((float)m_Width, (float)m_Height); }

private:
	// 프록시처럼 윈도우 API를 공유ㅜ하지는 않지만
	// 공통적으로 실행되어야하는것은 맞다.
	void Update();
	ATOM Register_Class();
	BOOL Init_Inst();

public:
	StateManager this_StateManager;

private:
	KDevice m_Device;

public:
	KDevice& device() { return m_Device; }
	bool Init_Device();



public:
	RenderManager this_RenderManager;


protected:
	KWindow(const wchar_t* _Name);
	KWindow(const wchar_t* _Name, HWND _hWnd);
	~KWindow();
};

