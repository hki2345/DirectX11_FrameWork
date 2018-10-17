#pragma once
#include "TabDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

// ActorDlg ��ȭ �����Դϴ�.

class ActorDlg : public TabDlg
{
	DECLARE_DYNAMIC(ActorDlg)

public:
	ActorDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ActorDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACTORDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
