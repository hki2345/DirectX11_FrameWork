#pragma once
#include "TabDlg.h"

// Dlg_Editor ��ȭ �����Դϴ�.

class Dlg_Editor : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Editor)

public:
	Dlg_Editor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Editor();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
