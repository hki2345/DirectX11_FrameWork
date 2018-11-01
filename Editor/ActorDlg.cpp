// ActorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "ActorDlg.h"
#include "afxdialogex.h"
#include <HSpRenderer.h>
#include "Edit_Class.h"
#include <KCollider2D.h>
#include "View_Component.h"
#include <IsoMapRender.h>
#include <IsoEditor.h>


// ActorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(ActorDlg, CDialogEx)

ActorDlg::ActorDlg(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_ACTORDLG, pParent)
{
	Edit_Class::ADlg = this;
}

ActorDlg::~ActorDlg()
{
}

void ActorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACTORTREE, m_ObjectTree);
	DDX_Control(pDX, IDC_COMVIEWRANGE, m_ComRange);
}


BEGIN_MESSAGE_MAP(ActorDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ActorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ActorDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CREACTOR, &ActorDlg::OnBnClickedCreactor)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_SELCHANGED, IDC_ACTORTREE, &ActorDlg::OnTvnSelchangedActortree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_ACTORTREE, &ActorDlg::OnTvnBegindragActortree)
	ON_NOTIFY(TVN_KEYDOWN, IDC_ACTORTREE, &ActorDlg::OnTvnKeydownActortree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_ACTORTREE, &ActorDlg::OnTvnEndlabeleditActortree)
	ON_NOTIFY(NM_RCLICK, IDC_ACTORTREE, &ActorDlg::OnNMRClickActortree)
	ON_NOTIFY(NM_CLICK, IDC_ACTORTREE, &ActorDlg::OnNMClickActortree)
END_MESSAGE_MAP()


// ActorDlg 메시지 처리기입니다.


void ActorDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ActorDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ActorDlg::OnBnClickedCreactor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == Core_Class::MainScene())
	{
		return;
	}

	if (nullptr == m_SelectObject)
	{
		HTREEITEM item = m_ObjectTree.InsertItem(L"GameObject");

		KPtr<TheOne> pActor = Core_Class::MainScene()->Create_One(L"GameObject");


		KPtr<IsoMapRender> IsoRender = pActor->Add_Component<IsoMapRender>(L"Tile.png", KVector2(128.0f, 64.0f));
		IsoRender->CreateTile(0, 0);
		IsoRender->CreateTile(1, 0);


		//for (int y = -2; y < 2; y++)
		//{
		//	for (int x = -2; x < 2; x++)
		//	{
		//		IsoRender->CreateTile(x, y);
		//	}
		//}

		IsoRender->CreateTile(0, 0);
		IsoRender->CreateTile(1, 0);

		pActor->Add_Component<IsoEditor>();
		m_ObjectTree.SetItemData(item, (DWORD_PTR)((TheOne*)pActor));

	}
	else {
		HTREEITEM item = m_ObjectTree.InsertItem(L"GameObject", m_SelectObject);
		m_ObjectTree.Expand(m_SelectObject, TVE_EXPAND);

		KPtr<TheOne> pActor = Core_Class::MainScene()->Create_One(L"GameObject");
		pActor->Add_Component<KCollider2D>();

		KPtr<HSpRenderer> SpriteRender = pActor->Add_Component<HSpRenderer>();
		SpriteRender->Image(L"Rock.png");
		pActor->Trans()->scale_local({ 100, 100, 1 });
		pActor->Trans()->pos_local({ 0, 0, 5 });

		m_ObjectTree.SetItemData(item, (DWORD_PTR)((TheOne*)pActor));
	}
}

void ActorDlg::ResetObjectTreeItem() {

	Core_Class::MainSceneMgr().Change_State(Edit_Class::SceneDlg->m_CurSceneName);

	m_ObjectTree.DeleteAllItems();

	std::unordered_map<int, std::list<KPtr<TheOne>>> AllActor = Core_Class::next_state()->AllActor();

	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_StartMapIter = AllActor.begin();
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_EndMapIter = AllActor.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		std::list<KPtr<TheOne>>::iterator m_StartListIter = m_StartMapIter->second.begin();
		std::list<KPtr<TheOne>>::iterator m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			HTREEITEM item = m_ObjectTree.InsertItem((*m_StartListIter)->Name());
			m_ObjectTree.SetItemData(item, (DWORD_PTR)((TheOne*)(*m_StartListIter)));
			// (*m_StartListIter)
		}
	}
}

void ActorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	TabDlg::OnShowWindow(bShow, nStatus);

	if (bShow == SW_HIDE)
	{
		return;
	}

	if (nullptr == Core_Class::MainSceneMgr().Find_State(Edit_Class::SceneDlg->m_CurSceneName))
	{
		return;
	}

	ResetObjectTreeItem();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void ActorDlg::OnTvnSelchangedActortree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	RECT rc;
	m_ObjectTree.GetItemRect(pNMTreeView->itemNew.hItem, &rc, TRUE);

	POINT Pos;
	GetCursorPos(&Pos);
	m_ObjectTree.ScreenToClient(&Pos);

	if (Pos.x < rc.left || Pos.x > rc.right || Pos.y < rc.top || Pos.y > rc.bottom)
	{
		m_SelectObject = nullptr;
		m_ObjectTree.Select(nullptr, TVGN_CARET);
		return;
	}

	m_SelectObject = pNMTreeView->itemNew.hItem;

	TheOne* pActor = (TheOne*)m_ObjectTree.GetItemData(m_SelectObject);

	if (nullptr != pActor)
	{
		Edit_Class::m_gVIewCom->Release_AllDlg();
		Edit_Class::m_gVIewCom->Set_One(pActor);
	}
	// rc.left
	

}


void ActorDlg::OnTvnBegindragActortree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int a = 0;
}


void ActorDlg::OnTvnKeydownActortree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (VK_DELETE == pTVKeyDown->wVKey && nullptr != m_SelectObject)
	{
		TheOne* pActor = (TheOne*)m_ObjectTree.GetItemData(m_SelectObject);

		if (nullptr != pActor)
		{
			Edit_Class::m_gVIewCom->Release_AllDlg();
			pActor->Set_Death();
			// KPtr<HSpRenderer> Ptr = pActor->Get_Component<HSpRenderer>();
/*
			KPtr<KCollider2D> Ptr = pActor->Get_Component<KCollider2D>();

			if (nullptr != Ptr)
			{
				Ptr->Set_Death();
			}*/

		}

		m_ObjectTree.DeleteItem(m_SelectObject);
		m_ObjectTree.SelectItem(nullptr);
	}
}


void ActorDlg::OnTvnEndlabeleditActortree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	TheOne* pActor = (TheOne*)m_ObjectTree.GetItemData(m_SelectObject);

	if (nullptr != pActor && nullptr != pTVDispInfo->item.pszText)
	{
			pActor->Name(pTVDispInfo->item.pszText);
			m_ObjectTree.SetItemText(m_SelectObject, pTVDispInfo->item.pszText);
	}
}


BOOL ActorDlg::OnInitDialog()
{
	TabDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CCreateContext         m_context;

	m_context.m_pNewViewClass = RUNTIME_CLASS(View_Component);

	View_Component* pView = (View_Component*)((CFrameWnd*)this)->CreateView(&m_context);

	RECT ComViewSize = { 200, 10, 700, 530 };
	pView->ShowWindow(SW_SHOW);
	pView->MoveWindow(&ComViewSize);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ActorDlg::OnNMRClickActortree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (nullptr == m_SelectObject)
	{
		return;
	}

	RECT rc;
	m_ObjectTree.GetItemRect(m_SelectObject, &rc, TRUE);

	POINT Pos;
	GetCursorPos(&Pos);
	m_ObjectTree.ScreenToClient(&Pos);

	if (Pos.x < rc.left || Pos.x > rc.right || Pos.y < rc.top || Pos.y > rc.bottom)
	{
		m_SelectObject = nullptr;
		Edit_Class::m_gVIewCom->Release_AllDlg();
		m_ObjectTree.Select(nullptr, TVGN_CARET);
		return;
	}
}


void ActorDlg::OnNMClickActortree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	if (nullptr == m_SelectObject)
	{
		return;
	}


	RECT rc;
	m_ObjectTree.GetItemRect(m_SelectObject, &rc, TRUE);

	POINT Pos;
	GetCursorPos(&Pos);
	m_ObjectTree.ScreenToClient(&Pos);

	if (Pos.x < rc.left || Pos.x > rc.right || Pos.y < rc.top || Pos.y > rc.bottom)
	{
		Edit_Class::m_gVIewCom->Release_AllDlg();
		m_ObjectTree.Select(nullptr, TVGN_CARET);
		return;
	}
}
