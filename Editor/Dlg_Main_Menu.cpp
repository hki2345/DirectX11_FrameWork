// Dlg_Main_Menu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_Main_Menu.h"
#include "afxdialogex.h"

// #include "Dlg_StateTab.h"
// #include "Dlg_SpriteTab.h"
// #include "Dlg_TheOneTab.h"
#include "Dlg_Tool.h"

#include "Edit_Class.h"
// Dlg_Main_Menu ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_Main_Menu, CDialogEx)

Dlg_Main_Menu::Dlg_Main_Menu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAIN_MENU, pParent), m_CurTab(0)
{
	Edit_Class::MenuDlg = this;
}

Dlg_Main_Menu::~Dlg_Main_Menu()
{
	for (size_t i = 0; i < m_Dlgvec.size(); i++)
	{
		delete m_Dlgvec[i];
	}
}


void Dlg_Main_Menu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, TabMenu);
}


BEGIN_MESSAGE_MAP(Dlg_Main_Menu, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dlg_Main_Menu::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Dlg_Main_Menu::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &Dlg_Main_Menu::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// Dlg_Main_Menu �޽��� ó�����Դϴ�.




void Dlg_Main_Menu::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnOK();
}


void Dlg_Main_Menu::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// CDialogEx::OnCancel();
}


BOOL Dlg_Main_Menu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(KColor::White.color_to_reverse255(), TRUE);

	SetCursor(LoadCursor(nullptr, IDC_ARROW));




	Create_Dialog<Dlg_Tool>(IDD_DLG_TOOL, L"Tool", true);
	// Create_Dialog<Dlg_StateTab>(IDD_STATE, L"State", true);
	// Create_Dialog<Dlg_TheOneTab>(IDD_THEONE, L"TheOne", false);
	// Create_Dialog<Dlg_SpriteTab>(IDD_SPRITE, L"Sprite", true);
	
	show(0);

	return TRUE;

}

void Dlg_Main_Menu::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// GetcurSel -> ���� ���� ����Ŵ -> ��ȭ�Ѵٸ� �װ��� �� ���� ����Ŵ
	show(TabMenu.GetCurSel());

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void Dlg_Main_Menu::show(size_t _Inx)
{
	if (true == m_Dlgvec[_Inx]->m_StateChange)
	{
		Core_Class::Main_StateManager().change_state(m_Dlgvec[_Inx]->StateName.GetString());
	}
	else
	{
		if (true == m_Dlgvec[_Inx]->m_StateChange)
		{
			Core_Class::Main_StateManager().change_state(nullptr);
		}
	}

	m_Dlgvec[m_CurTab]->ShowWindow(SW_HIDE);
	m_Dlgvec[_Inx]->ShowWindow(SW_SHOW);
	TabMenu.SetCurSel((int)_Inx);

	m_CurTab = _Inx;
}

void Dlg_Main_Menu::Init()
{
	// Create_Dialog<Dlg_StateTab>(IDD_STATE, L"State", true);
	// Create_Dialog<Dlg_TheOneTab>(IDD_THEONE, L"TheOne", false);
	// Create_Dialog<Dlg_SpriteTab>(IDD_SPRITE, L"Sprite", true);
	// show(0);
}