#pragma once
#include "TabDlg.h"

// HToolDlg ��ȭ �����Դϴ�.

class HToolDlg : public TabDlg
{
	DECLARE_DYNAMIC(HToolDlg)

public:
	HToolDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~HToolDlg();

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
