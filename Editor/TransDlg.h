#pragma once
#include "ComDlg.h"
#include <TransPosition.h>
#include "DropEditor.h"

// TransDlg 대화 상자입니다.

class TransDlg : public ComDlg
{
	DECLARE_DYNAMIC(TransDlg)

public:
	TransDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TransDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSCOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnFocusTransValue(UINT _Id);
	afx_msg void OnEnChangeTransValue(UINT _Id);

public:
	// float m_ArrTrans[3][3];

	enum EDITORVECTOR
	{
		EV_POS = 0,
		EV_ROT,
		EV_SCAL
	};

	TransPosition* m_pTrans;

	KMatrix TransValue;
	CEdit ArrMatValue[3][3];

	DropEditor NameXYZ[3][3];

public:
	void SettingComponent(Component* _pCom);

	void Update_Trans();
};
