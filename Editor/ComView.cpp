// ComView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "ComView.h"
#include "TVAR.h"
#include "AddCom.h"



// ComView

IMPLEMENT_DYNCREATE(ComView, CScrollView)

ComView::ComView() : m_pActor(nullptr)
{
	TVAR::g_ComView = this;
}

ComView::~ComView()
{
	AllDlgDestroy();
}


BEGIN_MESSAGE_MAP(ComView, CScrollView)
END_MESSAGE_MAP()


// ComView 그리기입니다.

void ComView::AllDlgDestroy() 
{
	m_pActor = nullptr;
	StartDlgiter = m_ComDlgList.begin();
	EndDlgiter = m_ComDlgList.end();

	for (; StartDlgiter != EndDlgiter; ++StartDlgiter)
	{
		if (nullptr != (*StartDlgiter))
		{
			if (nullptr != (*StartDlgiter)->m_hWnd)
			{
				(*StartDlgiter)->ShowWindow(SW_HIDE);
			}
			delete (*StartDlgiter);
		}
	}

	m_ComDlgList.clear();
}

void ComView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);

}

void ComView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// ComView 진단입니다.

#ifdef _DEBUG
void ComView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void ComView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// ComView 메시지 처리기입니다.
void ComView::ActorSetting(HActor* _pActor)
{
	m_pActor = _pActor;

	Reset();
}


void ComView::Reset() 
{
	HActor* m_TempActor = m_pActor;

	AllDlgDestroy();

	m_pActor = m_TempActor;

	if (nullptr == m_pActor)
	{
		return;
	}

	std::list<KPtr<HComponent>> List = m_pActor->AllComList();

	std::list<KPtr<HComponent>>::iterator StartIter = List.begin();
	std::list<KPtr<HComponent>>::iterator EndIter = List.end();

	POINT Pos = { 0, 0 };
	POINT Size = { 480, 0 };

	AddCom* NewDlg = new AddCom();
	NewDlg->m_pActor = m_pActor;
	NewDlg->Create(IDD_ADDCOM, this);
	NewDlg->ShowWindow(SW_SHOW);
	NewDlg->SetWindowPos(this, Pos.x, Pos.y, Size.x, NewDlg->SIZEY, SWP_NOZORDER);
	Pos.y += NewDlg->SIZEY;
	m_ComDlgList.push_back(NewDlg);

	for (; StartIter != EndIter; ++StartIter)
	{
		ComDlg* NewDlg = CreateDlg((*StartIter)->TypeInfo());
		NewDlg->ShowWindow(SW_SHOW);
		NewDlg->SetWindowPos(this, Pos.x, Pos.y, Size.x, NewDlg->SIZEY, SWP_NOZORDER);
		NewDlg->SettingComponent(*StartIter);
		Pos.y += NewDlg->SIZEY;

		m_ComDlgList.push_back(NewDlg);
	}

	SIZE sizeTotal;

	sizeTotal.cx = Size.x;
	sizeTotal.cy = Pos.y;
	SetScrollSizes(MM_TEXT, sizeTotal);
}