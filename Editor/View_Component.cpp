// View_Component.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "View_Component.h"
#include "Edit_Class.h"
#include "AddCom.h"



// View_Component

IMPLEMENT_DYNCREATE(View_Component, CScrollView)

View_Component::View_Component() : m_pActor(nullptr)
{
	Edit_Class::g_ComView = this;
}

View_Component::~View_Component()
{
	AllDlgDestroy();
}


BEGIN_MESSAGE_MAP(View_Component, CScrollView)
END_MESSAGE_MAP()


// View_Component �׸����Դϴ�.

void View_Component::AllDlgDestroy() 
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

void View_Component::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 0;
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
void View_Component::ActorSetting(TheOne* _pActor)
{
	m_pActor = _pActor;

	Reset();
}


void View_Component::Reset() 
{
	TheOne* m_TempActor = m_pActor;

	AllDlgDestroy();

	m_pActor = m_TempActor;

	if (nullptr == m_pActor)
	{
		return;
	}

	std::list<KPtr<Component>> List = m_pActor->list_component();

	std::list<KPtr<Component>>::iterator StartIter = List.begin();
	std::list<KPtr<Component>>::iterator EndIter = List.end();

	POINT Pos = { 0, 0 };
	POINT Size = { 480, 0 };

	// �ϴ� �ִ� ���۳�Ʈ ���ϴ� ���
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