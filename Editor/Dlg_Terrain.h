#pragma once
#include "TabDlg.h"



// Dlg_Terrain 대화 상자입니다.

class Dlg_Terrain : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Terrain)

public:
	Dlg_Terrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Terrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINDLG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public:
	void Init_Dlg() override;

	DECLARE_MESSAGE_MAP()
};
