#pragma once
#include "TabDlg.h"



// Dlg_Terrain ��ȭ �����Դϴ�.

class Dlg_Terrain : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Terrain)

public:
	Dlg_Terrain(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Terrain();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINDLG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


public:
	void Init_Dlg() override;

	DECLARE_MESSAGE_MAP()
};
