#pragma once
#include "ComDlg.h"

// NoneComDlg 대화 상자입니다.

class NoneComDlg : public ComDlg
{
	DECLARE_DYNAMIC(NoneComDlg)

public:
	NoneComDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~NoneComDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NONECOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
};
