#pragma once
#include "TabDlg.h"



// Dlg_MeshConv ��ȭ �����Դϴ�.

class Dlg_MeshConv : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_MeshConv)

public:
	Dlg_MeshConv(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_MeshConv();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
