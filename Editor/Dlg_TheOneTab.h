#pragma once
#include "TabState.h"
#include "Mouse_Droper.h"

#include "afxcmn.h"
#include "afxwin.h"

// Dlg_TheOneTab ��ȭ �����Դϴ�.

class Dlg_TheOneTab : public TabState
{
	DECLARE_DYNAMIC(Dlg_TheOneTab)

public:
	Dlg_TheOneTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_TheOneTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THEONE };
#endif

protected:
	Mouse_Droper m_Droper;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
