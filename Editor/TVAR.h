#pragma once
#include <HScene.h>
#include "SpriteDlg.h"
#include "MenuDlg.h"
#include "SceneDlg.h"
#include "ActorDlg.h"
#include "ComView.h"

class TVAR
{
public:
	static HPTR<HScene> SelectScene;
	static MenuDlg* MDlg;
	static SceneDlg* SceneDlg;
	static SpriteDlg* SpDlg;
	static ActorDlg* ADlg;
	static ComView* g_ComView;

private:
	TVAR();
	~TVAR();
};

