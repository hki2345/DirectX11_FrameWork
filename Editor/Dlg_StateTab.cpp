// Dlg_SceneTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_StateTab.h"
#include "afxdialogex.h"
#include "EditUpdater.h"

#include <ResourceManager.h>

#include <Core_Class.h>
#include <Container_Class.h>
#include "Edit_Class.h"


#include <Renderer_Sprite.h>
#include <Collider2D_DE.h>

#include <GameData.h>

// Dlg_SceneTab ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_StateTab, TabState)

Dlg_StateTab::Dlg_StateTab(CWnd* pParent /*=NULL*/)
	: TabState(IDD_STATE, pParent),
	m_CurState_Name(_T("ơơ")),
	m_State_Name(_T("ơơ"))
{
	Edit_Class::StateTab = this;
}

Dlg_StateTab::~Dlg_StateTab()
{
}

void Dlg_StateTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CURSCENENAME, m_CurState_Name);
	DDX_Text(pDX, IDC_SCENE_NAME, m_State_Name);
	DDX_Control(pDX, IDC_SCENELIST, m_State_List);
}


BEGIN_MESSAGE_MAP(Dlg_StateTab, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dlg_StateTab::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Dlg_StateTab::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SCENECREATOR, &Dlg_StateTab::OnBnClickedScenecreator)
	ON_LBN_SELCHANGE(IDC_SCENELIST, &Dlg_StateTab::OnLbnSelchangeScenelist)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// Dlg_SceneTab �޽��� ó�����Դϴ�.

void Dlg_StateTab::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


void Dlg_StateTab::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnCancel();
}




void Dlg_StateTab::OnBnClickedScenecreator()
{
	UpdateData(TRUE);

	// ���� ó��
	if (m_State_Name == L"")
	{
		// ���â
		AfxMessageBox(L"�̸��� ������");
		return;
	}

	CString TempString = m_State_Name;
	TempString.Trim();

	if (TempString == L"")
	{
		// ���â
		AfxMessageBox(L"���⸸ �ִ� �� �̸��� �ƴ�");
		return;
	}

	KPtr<State> NewState = Core_Class::Main_StateManager().find_state(m_State_Name.GetString());

	if (nullptr != NewState)
	{
		AfxMessageBox(L"�̹� �ִ� ������Ʈ��");
		return;
	}

	m_State_List.AddString(m_State_Name);

	NewState = Core_Class::Main_StateManager().create_state(m_State_Name.GetString());
	NewState->Create_Updater<EditUpdater>();
	Core_Class::Main_StateManager().change_state(m_State_Name.GetString());

	UpdateData(FALSE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void Dlg_StateTab::OnLbnSelchangeScenelist()
{
	UpdateData(TRUE);

	m_State_List.GetText(m_State_List.GetCurSel(), m_CurState_Name);
	Edit_Class::Cur_State = Core_Class::Main_StateManager().find_state(m_CurState_Name.GetString());

	if (nullptr == Edit_Class::Cur_State)
	{
		m_CurState_Name = L"����";
		return;
	}

	Core_Class::Main_StateManager().change_state(m_CurState_Name.GetString());

	UpdateData(FALSE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void Dlg_StateTab::Init()
{
	// OnBnClickedScenecreator();
}


void Dlg_StateTab::OnShowWindow(BOOL bShow, UINT nStatus)
{
	TabState::OnShowWindow(bShow, nStatus);

	if (bShow == SW_HIDE)
	{
		return;
	}

	if (nullptr == Core_Class::Main_StateManager().find_state(Edit_Class::StateTab->m_CurState_Name))
	{
		return;
	}

	Core_Class::Main_StateManager().change_state(m_CurState_Name);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
