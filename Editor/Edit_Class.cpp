#include "stdafx.h"
#include "Edit_Class.h"

KPtr<State> Edit_Class::Select_State = nullptr;
MenuDlg* Edit_Class::MDlg = nullptr;
SpriteDlg* Edit_Class::SpDlg = nullptr;
SceneDlg* Edit_Class::SceneDlg = nullptr;
ActorDlg* Edit_Class::ADlg = nullptr;
ComView* Edit_Class::g_ComView = nullptr;

Edit_Class::Edit_Class()
{
}


Edit_Class::~Edit_Class()
{
}
