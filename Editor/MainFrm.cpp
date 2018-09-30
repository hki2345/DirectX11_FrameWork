
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Editor.h"

#include "MainFrm.h"

#include "View_Main_Frame.h"
#include "View_Main_Menu.h"

#include <KCore.h>
#include "EditLauncher.h"
#include "Edit_Class.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ������ �̷��� ����
	//// �������� Ŭ���̾�Ʈ ������ �����ϴ� �並 ����ϴ�.
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("�� â�� ������ ���߽��ϴ�.\n");
	//	return -1;
	//}

	m_Main.lock();

	m_Main.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE);

	m_Main.CreateView(0, 0, RUNTIME_CLASS(View_Main_Frame), SIZE{ 800, 600 }, nullptr);

	CWnd* pWnd = m_Main.GetPane(0, 0);

	KCore::Start<EditLauncher>(AfxGetInstanceHandle(), L"Angry Editor", pWnd->m_hWnd);
	m_Main.CreateView(0, 1, RUNTIME_CLASS(View_Main_Menu), SIZE{ 500, 600 }, nullptr);


	RECT newRc = { 0, 0, 1600, 600 };
	
	// ������ ����� �°� ����
	AdjustWindowRect(&newRc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(nullptr, 0, 0, newRc.right, newRc.bottom - newRc.top + m_Main.borderY() * 2, SWP_NOMOVE | SWP_NOZORDER);


	Edit_Class::MenuDlg->Init();
	// Edit_Class::SpriteTab->Init();
	//Edit_Class::StateTab->Init();

	//Edit_Class::TheOneTab->Reset_OneTree();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.hMenu = nullptr;
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �� â���� ��Ŀ���� �̵��մϴ�.
	m_Main.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �信�� ù° ũ���� �ش� ��ɿ� ��Ÿ������ �մϴ�.
	if (m_Main.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// �׷��� ������ �⺻ ó���մϴ�.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

