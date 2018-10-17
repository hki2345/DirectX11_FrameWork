#include "HVAR.h"
#include "HMACRO.h"
#include "HWindow.h"
#include "HDevice.h"

HPTR<HWindow> HVAR::g_MainWindow = nullptr;

HVAR::HVAR()
{
}


HVAR::~HVAR()
{
}

void HVAR::MainWindow(const wchar_t* _Name)
{
	g_MainWindow = HWindow::FindHWindow(_Name);
	TASSERT(nullptr == g_MainWindow);
}

HWindow& HVAR::MainWindow() {
	return *g_MainWindow;
}

HSceneMgr& HVAR::MainSceneMgr()
{
	return g_MainWindow->SceneMgr;
}

HPTR<HScene> HVAR::MainScene() {
	return g_MainWindow->SceneMgr.CurScene();
}

HPTR<HScene> HVAR::NextScene() {
	return g_MainWindow->SceneMgr.NextScene();
}

HDevice& HVAR::MainDevice() {
	return g_MainWindow->Device();
}
ID3D11Device* HVAR::PDevice() {
	return g_MainWindow->Device().PDevice();
}
ID3D11DeviceContext* HVAR::Context() {
	return g_MainWindow->Device().Context();
}