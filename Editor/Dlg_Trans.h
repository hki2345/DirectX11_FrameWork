#pragma once
#include "Dlg_Component.h"
#include <TransPosition.h>
#include "Mouse_Droper.h"


// Dlg_Trans ��ȭ �����Դϴ�.

class Dlg_Trans : public Dlg_Component
{
	DECLARE_DYNAMIC(Dlg_Trans)

public:
	Dlg_Trans(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Trans();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnFocusTransValue(UINT _Id);
	afx_msg void OnEnChangeTransValue(UINT _Id);

	enum EDIT_VECTOR
	{
		ETV_POS = 0,
		ETV_ROTATE,
		ETV_SCALE,
	};

	CEdit TransMatValue[3][3];
	TransPosition* m_Trans;
	KMatrix TransMat;


	Mouse_Droper TransDroper[3][3];

public:
	virtual void set_component(Component* _Com);
	void Update_Trans();
	void Update_EditValue();
};
