// Dlg_MainMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "afxdialogex.h"

#include "Dlg_MainMenu.h"
#include "Dlg_Editor.h"
#include "Dlg_MeshConv.h"

#include "Dlg_Force.h"

#include "Edit_Class.h"


// Dlg_MainMenu 대화 상자입니다.

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
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &Dlg_MainMenu::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// Dlg_MainMenu 메시지 처리기입니다.

BOOL Dlg_MainMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255), TRUE);

	CreateDlg<Dlg_MeshConv>(IDD_MESHDLG, L"Mesh");
	CreateDlg<Dlg_Force>(IDD_FORCEDLG, L"Force");
	CreateDlg<Dlg_Editor>(IDD_TOOLDLG, L"Tool");
	Show_Dlg(0);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void Dlg_MainMenu::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// GetcurSel -> 현재 셀을 가리킴 -> 변화한다면 그것이 곧 셀을 가리킴
	Show_Dlg(TabMenu.GetCurSel());

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}



void Dlg_MainMenu::Show_Dlg(size_t _Index)
{

	if (true == m_VecDlg[_Index]->m_StateChange)
	{
		Core_Class::MainSceneMgr().Change_State(m_VecDlg[_Index]->StateName.GetString());
	}
	else {
		Core_Class::MainSceneMgr().Change_State(nullptr);
	}

	m_VecDlg[m_CurIndex]->ShowWindow(SW_HIDE);
	m_VecDlg[_Index]->ShowWindow(SW_SHOW);
	TabMenu.SetCurSel((int)_Index);

	m_CurIndex = _Index;
}