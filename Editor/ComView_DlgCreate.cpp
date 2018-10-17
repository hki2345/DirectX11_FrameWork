#include "stdafx.h"
#include "resource.h"
#include <HComponent.h>

#include "ComView.h"
#include "TransDlg.h"
#include "SpRenderDlg.h"
#include "NoneComDlg.h"

ComDlg* ComView::CreateDlg(const type_info* _Type)
{
	ComDlg* NewDlg = nullptr;

	if (_Type == &typeid(HTrans))
	{
		NewDlg = new TransDlg();
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