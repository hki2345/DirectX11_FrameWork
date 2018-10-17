#pragma once
#include "TabDlg.h"

// HToolDlg 대화 상자입니다.

class HToolDlg : public TabDlg
{
	DECLARE_DYNAMIC(HToolDlg)

public:
	HToolDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~HToolDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
