#pragma once
#include <State.h>
#include "Dlg_SpriteTab.h"
#include "Dlg_Main_Menu.h"
#include "Dlg_StateTab.h"
#include "Dlg_TheOneTab.h"
#include "Dlg_Trans.h"

#include "View_Component.h"

class Edit_Class
{
public:
	static KPtr<TheOne> SelectOne;
	static KPtr<TheOne> FocusOne;


	static KPtr<State>		Cur_State;
	static TabState*		Cur_Tab;

	static Dlg_Main_Menu*	MenuDlg;
	static Dlg_StateTab*	StateTab;
	static Dlg_SpriteTab*	SpriteTab;
	static Dlg_TheOneTab*	TheOneTab;

	static Dlg_Trans*		TransTab;

	static View_Component*	m_View_Component;

public:
	Edit_Class();
	~Edit_Class();
};

