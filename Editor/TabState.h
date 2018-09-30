#pragma once
#include "afxdialogex.h"
#include <Core_Class.h>

class TabState : public CDialogEx
{
public:
	CString StateName;
	bool m_StateChange;

public:
	TabState(const int& _id, CWnd* _Wnd);
	~TabState();
};

