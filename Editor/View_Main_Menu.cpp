// View_Main_Menu.cpp : 구현 파일입니다.
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


// View_Main_Menu 그리기입니다.

void View_Main_Menu::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// View_Main_Menu 진단입니다.

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


// View_Main_Menu 메시지 처리기입니다.
BOOL View_Main_Menu::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	m_MenuDLG.Create(IDD_MAIN_MENU, this);
	m_MenuDLG.ShowWindow(SW_SHOW);

	return 0;
}
