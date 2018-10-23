#include "stdafx.h"
#include "resource.h"
#include <Component.h>

#include "View_Component.h"
#include "Dlg_TransPosition.h"
#include "SpRenderDlg.h"
#include "NoneComDlg.h"

ComDlg* View_Component::CreateDlg(const type_info* _Type)
{
	ComDlg* NewDlg = nullptr;

	if (_Type == &typeid(TransPosition))
	{
		NewDlg = new Dlg_TransPosition();
		NewDlg->Create(IDD_TRANSCOM, this);
	}
	else if(_Type == &typeid(HSpRenderer)) {
		NewDlg = new SpRenderDlg();
		NewDlg->Create(IDD_SPRENDER, this);
	}
	else {
		NewDlg = new NoneComDlg();
		NewDlg->Create(IDD_NONECOM, this);
	}

	return NewDlg;
}