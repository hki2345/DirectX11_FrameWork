#pragma once
#include "afxcmn.h"
#include "TabState.h"

// Dlg_Tool ��ȭ �����Դϴ�.

class Dlg_Tool : public TabState
{
	DECLARE_DYNAMIC(Dlg_Tool)

public:
	Dlg_Tool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Tool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
