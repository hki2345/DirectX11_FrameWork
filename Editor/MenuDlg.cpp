// MenuDlg.cpp : 구현 파일입니다.
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


// MenuDlg 대화 상자입니다.

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


// MenuDlg 메시지 처리기입니다.


void MenuDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnOK();
}


void MenuDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

