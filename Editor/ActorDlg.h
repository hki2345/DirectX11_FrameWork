#pragma once
#include "TabDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

// ActorDlg 대화 상자입니다.

class ActorDlg : public TabDlg
{
	DECLARE_DYNAMIC(ActorDlg)

public:
	ActorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ActorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACTORDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCreactor();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTvnSelchangedActortree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindragActortree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydownActortree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditActortree(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CTreeCtrl m_ObjectTree;
	HTREEITEM m_SelectObject;
	void ResetObjectTreeItem();
	virtual BOOL OnInitDialog();
	CStatic m_ComRange;
	afx_msg void OnNMRClickActortree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickActortree(NMHDR *pNMHDR, LRESULT *pResult);
};
