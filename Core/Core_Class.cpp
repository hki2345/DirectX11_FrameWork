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


void Core_Class::Main_Window(const wchar_t* _Name)
{
	g_MainWindow = KWindow::Find_Window(_Name);
	KASSERT(nullptr == g_MainWindow);
}

KWindow& Core_Class::Main_Window()
{
	return *g_MainWindow;
}

StateManager& Core_Class::Main_StateManager()
{
	return g_MainWindow->this_StateManager;
}

KPtr<State> Core_Class::main_state()
{
	return g_MainWindow->this_StateManager.state();
}

KPtr<State> Core_Class::next_state()
{
	return g_MainWindow->this_StateManager.next_state();
}

KDevice& Core_Class::MainDevice()
{
	return g_MainWindow->Device();
}

ID3D11Device* Core_Class::Device()
{
	return g_MainWindow->Device().Device();
}

ID3D11DeviceContext* Core_Class::Context()
{
	return g_MainWindow->Device().Context();
}