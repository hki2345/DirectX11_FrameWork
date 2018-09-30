#pragma once
#include "afxcmn.h"
#include "TabState.h"

// Dlg_Tool 대화 상자입니다.

class Dlg_Tool : public TabState
{
	DECLARE_DYNAMIC(Dlg_Tool)

public:
	Dlg_Tool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Tool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
