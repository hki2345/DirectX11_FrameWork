#pragma once
#include "TabDlg.h"
#include "afxcmn.h"
#include "afxwin.h"



// Dlg_FbxLoad ��ȭ �����Դϴ�.
class Dlg_FbxLoad : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_FbxLoad)

public:
	Dlg_FbxLoad(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_FbxLoad();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FBXDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
