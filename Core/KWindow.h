#pragma once
#include "StateManager.h"
#include "RenderManager.h"
#include "KDevice.h"

#include <Windows.h>
#include <unordered_map>

// ������ â�� ������ ��� �� �ְ��ϸ�, �� â�� �ؽ��� �����Ѵ�.
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
	// ���� ũ����Ʈ ������ - ��𼭵� �����츦 ����� ���� �� �ְڴ�.
	static KPtr<KWindow> Create_Window(const wchar_t* _Name, HWND _hWnd = nullptr);
	static KPtr<KWindow> Find_Window(const wchar_t* _Name);
	static void Erase_Window(const HWND _Handle);
	static void Progress();

private:
	// �����찡 ��������� ��� �� ���Ͻø� �����ϰڴ�.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


public:
	// ī��Ʈ ���°�
	static size_t WindowCount()
	{
		return g_NWinMap.size();
	}

	///////////////////////////////////////////// Member

private:
	// ���� �����찡 ���� HWND�� HDC
	HWND m_HWnd;
	HDC m_Hdc;
	size_t m_Width;
	size_t m_Height;
	bool m_bFull;

public:
	HWND KWnd() { return m_HWnd; }
	HDC KHdc()	{ return m_Hdc; }

	// ���̷�Ʈ 11������ Full��尡 false�̰� â��尡 true�̴�. ���� �ѹ� �ٲ��� ��
	bool		Is_Full()	{ return !m_bFull; }
	size_t&		width()		{ return m_Width; }
	size_t&		heigth()	{ return m_Height; }
	float		widthf()	{ return (float)m_Width; }
	float		heigthf()	{ return (float)m_Height; }
	
	void Show(int _ShowOption = SW_SHOWDEFAULT);
	void size(size_t _X, size_t _Y);
	KVector2 size() const { return KVector2((float)m_Width, (float)m_Height); }

private:
	// ���Ͻ�ó�� ������ API�� ������������ ������
	// ���������� ����Ǿ���ϴ°��� �´�.
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

