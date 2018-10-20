#include "Core_Class.h"
#include "KMacro.h"
#include "KWindow.h"
#include "KDevice.h"

KPtr<KWindow> Core_Class::g_MainWindow = nullptr;

Core_Class::Core_Class()
{
}


Core_Class::~Core_Class()
{
}

void Core_Class::MainWindow(const wchar_t* _Name)
{
	g_MainWindow = KWindow::FindHWindow(_Name);
	KASSERT(nullptr == g_MainWindow);
}

KWindow& Core_Class::MainWindow() {
	return *g_MainWindow;
}

HSceneMgr& Core_Class::MainSceneMgr()
{
	return g_MainWindow->ThisStateManager;
}

KPtr<HScene> Core_Class::MainScene() {
	return g_MainWindow->ThisStateManager.CurScene();
}

KPtr<HScene> Core_Class::NextScene() {
	return g_MainWindow->ThisStateManager.NextScene();
}

KDevice& Core_Class::MainDevice() {
	return g_MainWindow->Device();
}
ID3D11Device* Core_Class::PDevice() {
	return g_MainWindow->Device().PDevice();
}
ID3D11DeviceContext* Core_Class::Context() {
	return g_MainWindow->Device().Context();
}