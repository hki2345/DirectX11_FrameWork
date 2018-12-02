#pragma once
#include "TabDlg.h"
#include "afxwin.h"
#include "afxcolorbutton.h"


// Dlg_Force ��ȭ �����Դϴ�.

class Dlg_Force : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Force)

public:
	Dlg_Force(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Force();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORCEDLG };
#endif


private:
	CListBox m_KM3List;
	CListBox m_ForceList;
	CEdit m_ForceName;
	CMFCColorButton m_ForceColor;

	KPtr<TheOne> m_CurOne;

	std::list<KPtr<TheOne>> m_OList;
	std::list<KPtr<TheOne>>::iterator m_SO;
	std::list<KPtr<TheOne>>::iterator m_EO;

private:
	void Init_KM3List();
	

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedForcecolor();
	afx_msg void OnLbnSelchangeForcelist();
	afx_msg void OnEnChangeForcename();
	afx_msg void OnLbnSelchangeForkm3list();
	afx_msg void OnBnClickedKm3resetbtn();
};
