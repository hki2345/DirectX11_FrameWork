#pragma once
#include "ComDlg.h"
#include "afxwin.h"
#include <HActor.h>

// AddCom ��ȭ �����Դϴ�.

class AddCom : public ComDlg
{
	DECLARE_DYNAMIC(AddCom)

public:
	AddCom(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~AddCom();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDCOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComListCombo;
	HActor*	  m_pActor;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedAddcom();

	void CreateCom(CString ComName);
};
