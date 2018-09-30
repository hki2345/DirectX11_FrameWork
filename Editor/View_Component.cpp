// View_Component.cpp : ���� �����Դϴ�.
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


// View_Component �׸����Դϴ�.

void View_Component::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void View_Component::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// View_Component �����Դϴ�.

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


// View_Component �޽��� ó�����Դϴ�.


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

		// �ٴڰ� ���ڸ� �ʿ��ϴ� -> �浹ü�� �������� �� ���������� ����
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