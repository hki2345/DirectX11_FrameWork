#pragma once


// Dlg_UnitList ��ȭ �����Դϴ�.

class Dlg_UnitList : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_UnitList)

public:
	Dlg_UnitList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_UnitList();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITLISTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkList1();
};
