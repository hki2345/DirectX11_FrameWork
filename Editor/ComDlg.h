#pragma once
#include <Component.h>

// ComDlg 대화 상자입니다.

class ComDlg : public CDialogEx
{
public:
	UINT SIZEY;

public:
	virtual void SettingComponent(Component* _pCom) {}

public:
	ComDlg(int _id, CWnd* _pWnd) : CDialogEx(_id, _pWnd) 
	{

	}
	~ComDlg() {	}
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcPaint();
};
