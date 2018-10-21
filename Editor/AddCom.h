#pragma once
#include "ComDlg.h"
#include "afxwin.h"
#include <TheOne.h>

// AddCom 대화 상자입니다.

class AddCom : public ComDlg
{
	DECLARE_DYNAMIC(AddCom)

public:
	AddCom(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~AddCom();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDCOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComListCombo;
	TheOne*	  m_pActor;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedAddcom();

	void CreateCom(CString ComName);
};
