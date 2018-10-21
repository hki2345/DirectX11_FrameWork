
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "AR14TOOL.h"

#include "MainFrm.h"

#include "LeftView.h"
#include "RightView.h"

#include <KCore.h>
#include <ResourceManager.h>
#include <KImage.h>
#include "ToolCoreBuilder.h"
#include "TVAR.h"
#include <InputManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("뷰 창을 만들지 못했습니다.\n");
	//	return -1;
	//}

	m_MainWnd.Lock();

	m_MainWnd.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE);

	m_MainWnd.CreateView(0, 0, RUNTIME_CLASS(LeftView), SIZE{ 800, 600 }, nullptr);
		
	CWnd* pWnd = m_MainWnd.GetPane(0, 0);
	KCore::Start<ToolCoreBuilder>(AfxGetInstanceHandle(), L"MainWindow", pWnd->m_hWnd);

	m_MainWnd.CreateView(0, 1, RUNTIME_CLASS(RightView), SIZE{ 500, 600 }, nullptr);

	RECT Rc = { 0, 0, 1600, 600 };

	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(nullptr, 0, 0, Rc.right, Rc.bottom - Rc.top + m_MainWnd.GetBoderSizeY() * 2, SWP_NOMOVE | SWP_NOZORDER);

	// TVAR::MDlg->DlgInit();
	// TVAR::SpDlg->SpriteInit();
	// TVAR::SceneDlg->Init();


	// TVAR::ADlg->ResetObjectTreeItem();


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.hMenu = nullptr;
	return TRUE;
}

// CMainFrame 진단

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


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	// m_wndView.SetFocus();
	m_MainWnd.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	//if (m_MainWnd.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;
	// ShowCursor(TRUE);
	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

