#pragma once
#include "DXContainer.h"
#include "KCore.h"
#include "SmartPtr.h"
#include "HSceneMgr.h"
#include "KWindow.h"
#include "KDevice.h"
#include "HScene.h"
#include "HFBX.h"

// 일종의 연관 시켜져야할 클래스들을 포인터로 들고 있음 - 꽤나 유용한 클래스
class Core_Class
{
public:
	friend KCore;

private:
	static KPtr<KWindow> g_MainWindow;
	static void MainWindow(const wchar_t* _Name);

public:
	static KWindow& MainWindow();
	static HSceneMgr& MainSceneMgr();
	static KPtr<HScene> MainScene();
	static KPtr<HScene> NextScene();

	static KDevice& MainDevice();
	static ID3D11Device* PDevice();
	static ID3D11DeviceContext*	Context();

public:
	Core_Class();
	~Core_Class();
};

