#pragma once


// Dlg_UnitList 대화 상자입니다.

class Dlg_UnitList : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_UnitList)

public:
	Dlg_UnitList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_UnitList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITLISTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkList1();
};
