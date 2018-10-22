// MenuDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "MenuDlg.h"
#include "afxdialogex.h"
//#include "SceneDlg.h"
//#include "SpriteDlg.h"
//#include "ActorDlg.h"
#include "HToolDlg.h"
#include "Edit_Class.h"


// MenuDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(MenuDlg, CDialogEx)

MenuDlg::MenuDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MENUDLG, pParent), m_CurIndex(0)
{
	Edit_Class::MDlg = this;
}

MenuDlg::~MenuDlg()
{
	for (size_t i = 0; i < m_VecDlg.size(); i++)
	{
		delete m_VecDlg[i];
	}
}

void MenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, TabMenu);
}


BEGIN_MESSAGE_MAP(MenuDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MenuDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MenuDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &MenuDlg::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// MenuDlg �޽��� ó�����Դϴ�.


void MenuDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnOK();
}


void MenuDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnCancel();
}


BOOL MenuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255), TRUE);

	CreateDlg<HToolDlg>(IDD_TOOLDLG, L"ToolDlg");
	//CreateDlg<ActorDlg>(IDD_ACTORDLG, L"ActorDlg", false);
	//CreateDlg<SpriteDlg>(IDD_SPRITEDLG, L"SpriteDlg");
	ShowDlg(0);
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void MenuDlg::ShowDlg(size_t _Index) 
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

void MenuDlg::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShowDlg(TabMenu.GetCurSel());

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

