// RightView.cpp : ���� �����Դϴ�.
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


// RightView �׸����Դϴ�.

void RightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// RightView �����Դϴ�.

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


// RightView �޽��� ó�����Դϴ�.


BOOL RightView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	MDlg.Create(IDD_MENUDLG, this);
	MDlg.ShowWindow(SW_SHOW);

	return 0;
}
