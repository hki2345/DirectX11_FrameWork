#pragma once
#include "TabState.h"
#include "Mouse_Droper.h"

#include "afxcmn.h"
#include "afxwin.h"

// Dlg_TheOneTab 대화 상자입니다.

class Dlg_TheOneTab : public TabState
{
	DECLARE_DYNAMIC(Dlg_TheOneTab)

public:
	Dlg_TheOneTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_TheOneTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THEONE };
#endif

protected:
	Mouse_Droper m_Droper;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTvnSelchangedOnetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindragOnetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydownOnetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditOnetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickOnetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickOnetree(NMHDR *pNMHDR, LRESULT *pResult);


	
public:
	CTreeCtrl m_TheOneTree;
	HTREEITEM m_SelectObject;
	HTREEITEM m_Launcher;
	virtual BOOL OnInitDialog();
	void Reset_OneTree();
};
