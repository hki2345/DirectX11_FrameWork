// RightView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "RightView.h"


// RightView

IMPLEMENT_DYNCREATE(RightView, CView)

RightView::RightView()
{

}

RightView::~RightView()
{
}

BEGIN_MESSAGE_MAP(RightView, CView)
END_MESSAGE_MAP()


// RightView 그리기입니다.

void RightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// RightView 진단입니다.

#ifdef _DEBUG
void RightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void RightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// RightView 메시지 처리기입니다.


BOOL RightView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	MDlg.Create(IDD_MENUDLG, this);
	MDlg.ShowWindow(SW_SHOW);

	return 0;
}
