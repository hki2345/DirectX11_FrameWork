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
	// 메인 윈도우를 지정하는 것은 코어에서밖에 할 수 없다.
	static KPtr<KWindow> g_MainWindow;
	static void Main_Window(const wchar_t* _Name);

	//  메인 윈도우와 그 스테이트 메니저는 짜피 하나뿐이겠다.
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

