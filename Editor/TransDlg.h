#pragma once
#include "ComDlg.h"
#include <TransPosition.h>
#include "DropEditor.h"

// TransDlg ��ȭ �����Դϴ�.

class TransDlg : public ComDlg
{
	DECLARE_DYNAMIC(TransDlg)

public:
	TransDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TransDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSCOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
