#pragma once
#include "DirectXHeader.h"
#include "SmartPtr.h"

#include "KWindow.h"
#include "StateManager.h"
#include "KDevice.h"
#include "KCore.h"

class Core_Class
{
public:
	friend KCore;

private:
	// ���� �����츦 �����ϴ� ���� �ھ���ۿ� �� �� ����.
	static KPtr<KWindow> g_MainWindow;
	static void Main_Window(const wchar_t* _Name);

	//  ���� ������� �� ������Ʈ �޴����� ¥�� �ϳ����̰ڴ�.
public:
	static KWindow& Main_Window();
	static StateManager& Main_StateManager();
	static KPtr<State> main_state();
	static KPtr<State> next_state();

	static KDevice&					MainDevice();
	static ID3D11Device*			Device();
	static ID3D11DeviceContext*		Context();

public:
	Core_Class();
	~Core_Class();
};

