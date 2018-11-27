#pragma once
#include "TabDlg.h"
#include "afxwin.h"



// Dlg_KM3Loader 대화 상자입니다.

class Dlg_KM3Loader : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_KM3Loader)

public:
	Dlg_KM3Loader(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_KM3Loader();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KM3DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void Hide_Dlg() override;
	void Update_RscTree();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKm3load();
	afx_msg void OnLbnSelchangeKm3list();
	CListBox m_KM3List;
	KPtr<TheOne> m_One;
};
