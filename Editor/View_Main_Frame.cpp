// View_Main_Frame.cpp : 구현 파일입니다.
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


// View_Main_Frame 그리기입니다.

void View_Main_Frame::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// View_Main_Frame 진단입니다.

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


// View_Main_Frame 메시지 처리기입니다.
