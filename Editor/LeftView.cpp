// LeftView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "LeftView.h"


// LeftView

IMPLEMENT_DYNCREATE(LeftView, CView)

LeftView::LeftView()
{

}

LeftView::~LeftView()
{
}

BEGIN_MESSAGE_MAP(LeftView, CView)
END_MESSAGE_MAP()


// LeftView �׸����Դϴ�.

void LeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// LeftView �����Դϴ�.

#ifdef _DEBUG
void LeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void LeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// LeftView �޽��� ó�����Դϴ�.
