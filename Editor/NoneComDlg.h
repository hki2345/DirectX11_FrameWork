#pragma once
#include "ComDlg.h"

// NoneComDlg ��ȭ �����Դϴ�.

class NoneComDlg : public ComDlg
{
	DECLARE_DYNAMIC(NoneComDlg)

public:
	NoneComDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NoneComDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NONECOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
};
