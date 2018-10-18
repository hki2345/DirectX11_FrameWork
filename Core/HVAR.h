#pragma once
#include "DHeader.h"
#include "HCore.h"
#include "SmartPtr.h"
#include "HSceneMgr.h"
#include "HWindow.h"
#include "HDevice.h"
#include "HScene.h"

class HVAR
{
public:
	friend HCore;

private:
	static KPtr<HWindow> g_MainWindow;
	static void MainWindow(const wchar_t* _Name);

public:
	static HWindow& MainWindow();
	static HSceneMgr& MainSceneMgr();
	static KPtr<HScene> MainScene();
	static KPtr<HScene> NextScene();

	static HDevice& MainDevice();
	static ID3D11Device* PDevice();
	static ID3D11DeviceContext*	Context();

public:
	HVAR();
	~HVAR();
};

