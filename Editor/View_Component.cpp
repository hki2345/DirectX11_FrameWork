// View_Component.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Editor.h"
#include "View_Component.h"
#include "Edit_Class.h"

// View_Component

IMPLEMENT_DYNCREATE(View_Component, CScrollView)

View_Component::View_Component() : m_One(nullptr)
{
	Edit_Class::m_View_Component = this;
}

View_Component::~View_Component()
{
	Dlg_Release();
}


BEGIN_MESSAGE_MAP(View_Component, CScrollView)
END_MESSAGE_MAP()


// View_Component 그리기입니다.

void View_Component::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void View_Component::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// View_Component 진단입니다.

#ifdef _DEBUG
void View_Component::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void View_Component::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// View_Component 메시지 처리기입니다.


void View_Component::set_one(TheOne* _Value)
{
	m_One = _Value;

	reset();
}
void  View_Component::Dlg_Release()
{
	m_One = nullptr;
	S_DlgIter = m_DlgCom_list.begin();
	E_DlgIter = m_DlgCom_list.end();

	for (; S_DlgIter != E_DlgIter; ++S_DlgIter)
	{
		if (nullptr != (*S_DlgIter))
		{
			if (nullptr != (*S_DlgIter)->m_hWnd)
			{
				(*S_DlgIter)->ShowWindow(SW_HIDE);
			}
			delete (*S_DlgIter);
		}
	}

	m_DlgCom_list.clear();
}

void View_Component::reset()
{
	TheOne* ThisOne = m_One;

	Dlg_Release();

	m_One = ThisOne;

	if (nullptr == m_One)
	{
		return;
	}

	std::list<KPtr<Component>> List = m_One->component_list();

	std::list<KPtr<Component>>::iterator StartIter = List.begin();
	std::list<KPtr<Component>>::iterator EndIter = List.end();

	POINT Pos = { 0, 0 };
	POINT Size = { 480, 0 };
/*
	Add_ComDlg* NewDlg = new Add_ComDlg();
	NewDlg->m_TheOne = m_One;
	NewDlg->Create(IDD_ADDCOMPO, this);
	NewDlg->ShowWindow(SW_SHOW);
	NewDlg->SetWindowPos(this, Pos.x, Pos.y, Size.x, NewDlg->Y_SIZE, SWP_NOZORDER);
	Pos.y += NewDlg->Y_SIZE;
	m_DlgCom_list.push_back(NewDlg);
*/
	for (; StartIter != EndIter; ++StartIter)
	{
		Dlg_Component* NewDlg = create_comDlg((*StartIter)->typeinfo());

		// 바닥과 판자만 필요하다 -> 충돌체와 디테일한 건 내부적으로 조정
		if (nullptr == NewDlg)
		{
			continue;
		}

		NewDlg->ShowWindow(SW_SHOW);
		NewDlg->SetWindowPos(this, Pos.x, Pos.y, Size.x, NewDlg->Y_SIZE, SWP_NOZORDER);
		NewDlg->set_component(*StartIter);
		Pos.y += NewDlg->Y_SIZE;

		m_DlgCom_list.push_back(NewDlg);
	}

	SIZE sizeTotal;

	sizeTotal.cx = Size.x;
	sizeTotal.cy = Pos.y;


	SetScrollSizes(MM_TEXT, sizeTotal);
}