#pragma once
#include "StateManager.h"
#include "KDevice.h"
#include "RenderManager.h"

#include <Windows.h>

// ������ â�� ������ ��� ���ְ� �ϸ�, �� â�� �ؽ��� ����
// unordered_map - �ؽ��� �����ϸ� ���� ������ ����.��
#include <unordered_map>



class KCore;
class KWindow : public Begin
{
public:
	friend KCore;

protected:
	KWindow(const wchar_t* _Name);
	KWindow(const wchar_t* _Name, HWND _hWnd);
	~KWindow();


	// ������ - �ٸ� ������� ������ �� �ִ� �и� ��
private:
	static std::unordered_map<std::wstring, KPtr<KWindow>>::iterator WinStartIter;
	static std::unordered_map<std::wstring, KPtr<KWindow>>::iterator WinEndIter;

	static std::unordered_map<std::wstring, KPtr<KWindow>> g_NWinMap;
	static std::unordered_map<HWND, KPtr<KWindow>> g_HWinMap;

private:
	// �� Ŭ������ �ν��Ͻ��� ��� ���� -> HWND ���� �Ŵ� ������ ������ �ƴ�
	// ����� ����־�� �� �༮���� �����
	// ��༮�� �� ���μ����� ���� ���� �и���
	static HINSTANCE g_HInst;
	static void Init(HINSTANCE _HInst);

public:
	static KPtr<KWindow> CreateHWindow(const wchar_t* _Name, HWND _hWnd = nullptr);
	static KPtr<KWindow> FindHWindow(const wchar_t* _Name);
	static void EraseHWindow(const HWND _Handle);
	static void Progress();

private:
	// �ش� �������� ���Ͻ� -> �޽��� �޴� ��
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static size_t WindowCount()
	{
		return g_NWinMap.size();
	}

	

	// ����� - �� â���� ������ �� ������ 

private:
	HWND m_HWnd;
	HDC m_Hdc;
	size_t m_Width;
	size_t m_Height;
	bool m_bFull;


public:

	// ���̷�Ʈ 11������ Full��尡 false�̰� â��尡 true�̴�. ���� �ѹ� �ٲ��� ��
	bool IsFull() { return !m_bFull; }

	HWND KHwnd() { return m_HWnd; }
	HDC KHdc() { return m_Hdc; }


	KUINT width_u() { return (KUINT)m_Width; }
	KUINT height_u() { return (KUINT)m_Height; }
	size_t width_st() { return m_Width; }
	size_t height_st() { return m_Height; }
	float width_f() { return (float)m_Width; }
	float height_f() { return (float)m_Height; }
	KVector2 size() { return{ width_f(), height_f() }; }
	void size(const size_t&_X, const size_t& _Y);

public:
	void Show(int _ShowOption = SW_SHOWDEFAULT);
	void FullScr_On();
	void FullScr_Off();

private:
	void Update();
	ATOM KRegisterClass();
	BOOL Init_Instance();

public:
	StateManager statemanager;

////////////////////////////////////////////////////////// ����̽�
// �����쿡 ����̽��� �����Ǵ� ����
private:
	KDevice m_Device;

public:
	KDevice& Device() { return m_Device; }

public:
	bool Init_Device();

private:
	RenderManager ThisRenderManager;

};
