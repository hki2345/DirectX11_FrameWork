// SceneDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "SceneDlg.h"
#include "afxdialogex.h"
#include <HVAR.h>
#include <HSpRenderer.h>
#include <HCol2D.h>
#include "TVAR.h"
#include <HResMgr.h>

#include "ToolSceneUpdater.h"


// SceneDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(SceneDlg, TabDlg)

SceneDlg::SceneDlg(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_SCENEDLG, pParent)
	, m_SceneName(_T("TestScene"))
	, m_CurSceneName(_T("TestScene"))
{
	TVAR::SceneDlg = this;
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


// SceneDlg �޽��� ó�����Դϴ�.


void SceneDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnOK();
}

void SceneDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnCancel();
}


void SceneDlg::OnBnClickedCrescene()
{
	UpdateData(TRUE);

	if (m_SceneName == L"")
	{
		AfxMessageBox(L"���̸� ����� �� ����־��.");
		return;
	}

	CString TestText = m_SceneName;
	TestText.Trim();

	if (TestText == L"")
	{
		AfxMessageBox(L"���鸸 �� �־� ���ٴ� �� ������ ���.");
		return;
	}

	KPtr<HScene> m_Scene = HVAR::MainSceneMgr().FindScene(m_SceneName.GetString());

	if (nullptr != m_Scene)
	{
		AfxMessageBox(L"�̹� �ִ� �̸��� ���� �� ������ ����.");
		return;
	}


	m_SceneList.AddString(m_SceneName);

	m_Scene = HVAR::MainSceneMgr().CreateScene(m_SceneName.GetString());
	m_Scene->CreateUpdater<ToolSceneUpdater>();
	HVAR::MainSceneMgr().ChangeScene(m_SceneName.GetString());

	KPtr<HActor> Camera = m_Scene->CreateActor();
	Camera->Trans()->LPos(HVEC(0.0f, 0.0f, -10.0f));
	KPtr<HCamera> CamCom = Camera->AddCom<HCamera>();
	CamCom->PushRenderLayer(0, 1, 2, 3, 4);


	UpdateData(FALSE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void SceneDlg::OnLbnSelchangeScenelist()
{
	UpdateData(TRUE);

	m_SceneList.GetText(m_SceneList.GetCurSel(), m_CurSceneName);

	TVAR::SelectScene = HVAR::MainSceneMgr().FindScene(m_CurSceneName.GetString());

	if (nullptr == TVAR::SelectScene)
	{
		m_CurSceneName = L"����� ����";
		return;
	}

	HVAR::MainSceneMgr().ChangeScene(m_CurSceneName.GetString());

	UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//void SceneDlg::OnBnClickedCreateobj()
//{
//	if (nullptr == HVAR::MainScene())
//	{
//		return;
//	}
//
//	 KPtr<HActor> pActor = HVAR::MainScene()->CreateActor(L"TestActor");
//	 KPtr<HSpRenderer> SpriteRender = pActor->AddCom<HSpRenderer>();
//	 SpriteRender->Image(L"Rock.png");
//	 pActor->Trans()->LScale({ 100, 100, 1 });
//	 pActor->Trans()->LPos({ 0, 0, 5 });
//
//	// pActor->OverSetting("Stage1", "Stage2", "Stage3");
//
//
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	if (nullptr == HVAR::MainSceneMgr().FindScene(TVAR::SceneDlg->m_CurSceneName))
	{
		return;
	}

	HVAR::MainSceneMgr().ChangeScene(m_CurSceneName);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void SceneDlg::OnBnClickedSavescene()
{
	CString DataPath = GamePath::FindPath(L"Data");

	DataPath += m_CurSceneName + L".SData";

	HVAR::MainScene()->Save(DataPath.GetString());

	// HVAR::MainScene()->Load(DataPath.GetString(), nullptr, nullptr, nullptr);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
