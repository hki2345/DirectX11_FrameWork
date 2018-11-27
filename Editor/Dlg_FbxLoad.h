#pragma once
#include "TabDlg.h"
#include "afxcmn.h"
#include "afxwin.h"



// Dlg_FbxLoad 대화 상자입니다.
class Dlg_FbxLoad : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_FbxLoad)

public:
	Dlg_FbxLoad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_FbxLoad();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FBXDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void Hide_Dlg() override;
	void Update_RscTree();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFbxloadbtn();
	CListBox m_RscList;
	KPtr<TheOne> m_One;
	afx_msg void OnLbnSelchangeFbxlist();
	afx_msg void OnBnClickedTokm2();
};
