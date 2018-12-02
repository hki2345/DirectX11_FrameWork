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
	void Update_Path();
	void Update_Tex(const TEX_TYPE& _Value, const int& _Inx);

	std::wstring Show_DDSWindow();

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_RscList;
	KPtr<TheOne> m_CurOne;
	CButton m_TexBtn[4];
	CEdit	m_TexPath[4];


	afx_msg void OnBnClickedFbxloadbtn();
	afx_msg void OnLbnSelchangeFbxlist();
	afx_msg void OnBnClickedTokm2();



	afx_msg void OnBnClickedDiffbtn();
	afx_msg void OnBnClickedNormbtn();
	afx_msg void OnBnClickedSpecbtn();
	afx_msg void OnBnClickedEmisbtn();
	
};
