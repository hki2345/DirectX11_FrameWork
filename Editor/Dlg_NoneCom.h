#pragma once
#include "Dlg_Component.h"

// Dlg_NoneCom ��ȭ �����Դϴ�.

class Dlg_NoneCom : public Dlg_Component
{
	DECLARE_DYNAMIC(Dlg_NoneCom)

public:
	Dlg_NoneCom(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_NoneCom();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NONECOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
