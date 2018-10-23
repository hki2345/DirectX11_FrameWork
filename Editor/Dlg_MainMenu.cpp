// Dlg_MainMenu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "Dlg_MainMenu.h"
#include "afxdialogex.h"
//#include "SceneDlg.h"
//#include "SpriteDlg.h"
//#include "ActorDlg.h"
#include "Dlg_Editor.h"
#include "Edit_Class.h"


// Dlg_MainMenu ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_MainMenu, CDialogEx)

Dlg_MainMenu::Dlg_MainMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MENUDLG, pParent), m_CurIndex(0)
{
	Edit_Class::MDlg = this;
}

Dlg_MainMenu::~Dlg_MainMenu()
{
	for (size_t i = 0; i < m_VecDlg.size(); i++)
	{
		delete m_VecDlg[i];
	}
}

void Dlg_MainMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, TabMenu);
}


BEGIN_MESSAGE_MAP(Dlg_MainMenu, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dlg_MainMenu::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Dlg_MainMenu::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &Dlg_MainMenu::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// Dlg_MainMenu �޽��� ó�����Դϴ�.


void Dlg_MainMenu::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnOK();
}


void Dlg_MainMenu::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnCancel();
}


BOOL Dlg_MainMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255), TRUE);

	CreateDlg<Dlg_Editor>(IDD_TOOLDLG, L"ToolDlg");
	//CreateDlg<ActorDlg>(IDD_ACTORDLG, L"ActorDlg", false);
	//CreateDlg<SpriteDlg>(IDD_SPRITEDLG, L"SpriteDlg");
	Show_Dlg(0);
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void Dlg_MainMenu::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// GetcurSel -> ���� ���� ����Ŵ -> ��ȭ�Ѵٸ� �װ��� �� ���� ����Ŵ
	Show_Dlg(TabMenu.GetCurSel());

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}



void Dlg_MainMenu::Show_Dlg(size_t _Index)
{

	if (true == m_VecDlg[_Index]->m_SceneChange)
	{
		Core_Class::MainSceneMgr().Change_State(m_VecDlg[_Index]->SceneName.GetString());
	}
	else {
		Core_Class::MainSceneMgr().Change_State(nullptr);
	}

	m_VecDlg[m_CurIndex]->ShowWindow(SW_HIDE);
	m_VecDlg[_Index]->ShowWindow(SW_SHOW);
	TabMenu.SetCurSel((int)_Index);

	m_CurIndex = _Index;
}