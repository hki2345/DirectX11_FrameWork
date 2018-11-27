#pragma once
#include "afxdialogex.h"
#include <Core_Class.h>

class TabDlg : public CDialogEx
{
public:
	CString SceneName;
	bool m_SceneChange;

public:
	virtual void Hide_Dlg();

public:
	TabDlg(int _id, CWnd* _pWnd);
	~TabDlg();
};

