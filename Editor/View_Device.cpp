// View_Device.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "View_Device.h"


// View_Device

IMPLEMENT_DYNCREATE(View_Device, CView)

View_Device::View_Device()
{

}

View_Device::~View_Device()
{
}

BEGIN_MESSAGE_MAP(View_Device, CView)
END_MESSAGE_MAP()


// View_Device 그리기입니다.

void View_Device::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// View_Device 진단입니다.

#ifdef _DEBUG
void View_Device::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void View_Device::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// View_Device 메시지 처리기입니다.
