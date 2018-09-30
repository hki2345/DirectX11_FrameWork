#include "stdafx.h"
#include "Edit_Class.h"


// 선택 - 남아있는 녀석, 관심 - 그때만 관심
KPtr<TheOne> Edit_Class::SelectOne = nullptr;
KPtr<TheOne> Edit_Class::FocusOne = nullptr;


KPtr<State> Edit_Class::Cur_State = nullptr;
TabState*	Edit_Class::Cur_Tab = nullptr;
Dlg_Main_Menu* Edit_Class::MenuDlg = nullptr;
Dlg_SpriteTab* Edit_Class::SpriteTab = nullptr;
Dlg_StateTab* Edit_Class::StateTab = nullptr;
Dlg_TheOneTab* Edit_Class::TheOneTab = nullptr;
View_Component* Edit_Class::m_View_Component = nullptr;
Dlg_Trans*		Edit_Class::TransTab = nullptr;

Edit_Class::Edit_Class()
{
}


Edit_Class::~Edit_Class()
{
}
