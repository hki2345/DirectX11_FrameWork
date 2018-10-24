#pragma once
#include <State.h>
#include "SpriteDlg.h"
#include "Dlg_MainMenu.h"
#include "SceneDlg.h"
#include "ActorDlg.h"
#include "View_Component.h"

class Edit_Class
{
public:
	static KPtr<State> Select_State;
	static Dlg_MainMenu* MDlg;
	static SceneDlg* SceneDlg;
	static SpriteDlg* SpDlg;
	static ActorDlg* ADlg;
	static View_Component* m_gVIewCom;

private:
	Edit_Class();
	~Edit_Class();
};

