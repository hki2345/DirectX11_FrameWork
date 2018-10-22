// SceneDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "SceneDlg.h"
#include "afxdialogex.h"
#include <Core_Class.h>
#include <HSpRenderer.h>
#include <HCol2D.h>
#include "Edit_Class.h"
#include <ResourceManager.h>

#include "Edit_SUpdater.h"


// SceneDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(SceneDlg, TabDlg)

SceneDlg::SceneDlg(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_SCENEDLG, pParent)
	, m_SceneName(_T("TestScene"))
	, m_CurSceneName(_T("TestScene"))
{
	Edit_Class::SceneDlg = this;
}

SceneDlg::~SceneDlg()
{
}

void SceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SCENENAME, m_SceneName);
	DDX_Control(pDX, IDC_SCENELIST, m_SceneList);
	DDX_Text(pDX, IDC_EDIT2, m_CurSceneName);
}


BEGIN_MESSAGE_MAP(SceneDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SceneDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SceneDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CRESCENE, &SceneDlg::OnBnClickedCrescene)
	ON_LBN_SELCHANGE(IDC_SCENELIST, &SceneDlg::OnLbnSelchangeScenelist)
	// ON_BN_CLICKED(IDC_CREATEOBJ, &SceneDlg::OnBnClickedCreateobj)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SAVESCENE, &SceneDlg::OnBnClickedSavescene)
END_MESSAGE_MAP()


// SceneDlg 메시지 처리기입니다.


void SceneDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnOK();
}

void SceneDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnCancel();
}


void SceneDlg::OnBnClickedCrescene()
{
	UpdateData(TRUE);

	if (m_SceneName == L"")
	{
		AfxMessageBox(L"씬이름 제대로 쳐 집어넣어라.");
		return;
	}

	CString TestText = m_SceneName;
	TestText.Trim();

	if (TestText == L"")
	{
		AfxMessageBox(L"공백만 쳐 넣어 놓다니 이 쓰레기 놈아.");
		return;
	}

	KPtr<State> m_Scene = Core_Class::MainSceneMgr().Find_State(m_SceneName.GetString());

	if (nullptr != m_Scene)
	{
		AfxMessageBox(L"이미 있는 이름의 씬을 쳐 만들지 마라.");
		return;
	}


	m_SceneList.AddString(m_SceneName);

	m_Scene = Core_Class::MainSceneMgr().Create_State(m_SceneName.GetString());
	m_Scene->CreateUpdater<Edit_SUpdater>();
	Core_Class::MainSceneMgr().Change_State(m_SceneName.GetString());

	KPtr<TheOne> Cam = m_Scene->Create_One();
	Cam->Trans()->pos_local(KVector4(0.0f, 0.0f, -10.0f));
	KPtr<Camera> CamCom = Cam->Add_Component<Camera>();
	CamCom->Insert_LayerData(0, 1, 2, 3, 4);


	UpdateData(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void SceneDlg::OnLbnSelchangeScenelist()
{
	UpdateData(TRUE);

	m_SceneList.GetText(m_SceneList.GetCurSel(), m_CurSceneName);

	Edit_Class::Select_State = Core_Class::MainSceneMgr().Find_State(m_CurSceneName.GetString());

	if (nullptr == Edit_Class::Select_State)
	{
		m_CurSceneName = L"현재씬 없음";
		return;
	}

	Core_Class::MainSceneMgr().Change_State(m_CurSceneName.GetString());

	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//void SceneDlg::OnBnClickedCreateobj()
//{
//	if (nullptr == Core_Class::MainScene())
//	{
//		return;
//	}
//
//	 KPtr<TheOne> pActor = Core_Class::MainScene()->Create_One(L"TestActor");
//	 KPtr<HSpRenderer> SpriteRender = pActor->AddCom<HSpRenderer>();
//	 SpriteRender->Image(L"Rock.png");
//	 pActor->Trans()->scale_local({ 100, 100, 1 });
//	 pActor->Trans()->LPos({ 0, 0, 5 });
//
//	// pActor->OverSetting("Stage1", "Stage2", "Stage3");
//
//
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


void SceneDlg::Init() 
{
	// OnBnClickedCrescene();
}


void SceneDlg::OnShowWindow(BOOL bShow, UINT nStatus)
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

	Core_Class::MainSceneMgr().Change_State(m_CurSceneName);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void SceneDlg::OnBnClickedSavescene()
{
	CString DataPath = PathManager::Find_ForderPath(L"Data");

	DataPath += m_CurSceneName + L".SData";

	Core_Class::MainScene()->Save(DataPath.GetString());

	// Core_Class::MainScene()->Load(DataPath.GetString(), nullptr, nullptr, nullptr);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
