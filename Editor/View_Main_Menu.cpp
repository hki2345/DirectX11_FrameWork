// View_Main_Menu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Editor.h"
#include "View_Main_Menu.h"


// View_Main_Menu

IMPLEMENT_DYNCREATE(View_Main_Menu, CView)

View_Main_Menu::View_Main_Menu()
{

}

View_Main_Menu::~View_Main_Menu()
{
}

BEGIN_MESSAGE_MAP(View_Main_Menu, CView)
END_MESSAGE_MAP()


// View_Main_Menu �׸����Դϴ�.

void View_Main_Menu::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// View_Main_Menu �����Դϴ�.

#ifdef _DEBUG
void View_Main_Menu::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void View_Main_Menu::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// View_Main_Menu �޽��� ó�����Դϴ�.
BOOL View_Main_Menu::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	m_MenuDLG.Create(IDD_MAIN_MENU, this);
	m_MenuDLG.ShowWindow(SW_SHOW);

	return 0;
}
