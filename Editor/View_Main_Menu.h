#pragma once
#include "Dlg_Main_Menu.h"

// View_Main_Menu ���Դϴ�.

class View_Main_Menu : public CView
{
	DECLARE_DYNCREATE(View_Main_Menu)

protected:
	View_Main_Menu();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~View_Main_Menu();

private:
	Dlg_Main_Menu m_MenuDLG;

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, 
		CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


