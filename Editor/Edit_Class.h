#pragma once
#include <State.h>
#include "SpriteDlg.h"
#include "MenuDlg.h"
#include "SceneDlg.h"
#include "ActorDlg.h"
#include "ComView.h"

class Edit_Class
{
public:
	static KPtr<State> Select_State;
	static MenuDlg* MDlg;
	static SceneDlg* SceneDlg;
	static SpriteDlg* SpDlg;
	static ActorDlg* ADlg;
	static ComView* g_ComView;

private:
	Edit_Class();
	~Edit_Class();
};

