#include "stdafx.h"
#include "resource.h"
#include <Component.h>

#include "View_Component.h"
#include "Dlg_Trans.h"

Dlg_Component* View_Component::create_comDlg(const type_info* _Type)
{
	Dlg_Component* NewDlg = nullptr;

	if (_Type == &typeid(TransPosition))
	{
		NewDlg = new Dlg_Trans();
		NewDlg->Create(IDD_TRANS, this);
	}

	return NewDlg;
}