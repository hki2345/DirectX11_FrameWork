#pragma once
#include <Component.h>

// ComDlg ��ȭ �����Դϴ�.

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
