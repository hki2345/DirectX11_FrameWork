// View_Main_Frame.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Editor.h"
#include "View_Main_Frame.h"


// View_Main_Frame

IMPLEMENT_DYNCREATE(View_Main_Frame, CView)

View_Main_Frame::View_Main_Frame()
{

}

View_Main_Frame::~View_Main_Frame()
{
}

BEGIN_MESSAGE_MAP(View_Main_Frame, CView)
END_MESSAGE_MAP()


// View_Main_Frame �׸����Դϴ�.

void View_Main_Frame::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// View_Main_Frame �����Դϴ�.

#ifdef _DEBUG
void View_Main_Frame::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void View_Main_Frame::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// View_Main_Frame �޽��� ó�����Դϴ�.
