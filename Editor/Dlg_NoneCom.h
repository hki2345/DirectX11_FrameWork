#pragma once
#include "Dlg_Component.h"

// Dlg_NoneCom 대화 상자입니다.

class Dlg_NoneCom : public Dlg_Component
{
	DECLARE_DYNAMIC(Dlg_NoneCom)

public:
	Dlg_NoneCom(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_NoneCom();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NONECOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
