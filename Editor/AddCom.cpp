// AddCom.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "AddCom.h"
#include "afxdialogex.h"
#include "Edit_Class.h"


// AddCom 대화 상자입니다.

IMPLEMENT_DYNAMIC(AddCom, CDialogEx)

AddCom::AddCom(CWnd* pParent /*=NULL*/)
	: ComDlg(IDD_ADDCOM, pParent)
{
	SIZEY = 100;
}

AddCom::~AddCom()
{
}

void AddCom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComListCombo);
}


BEGIN_MESSAGE_MAP(AddCom, CDialogEx)
	ON_BN_CLICKED(IDC_ADDCOM, &AddCom::OnBnClickedAddcom)
END_MESSAGE_MAP()


// AddCom 메시지 처리기입니다.


BOOL AddCom::OnInitDialog()
{
	ComDlg::OnInitDialog();

	UpdateData(TRUE);

	m_ComListCombo.AddString(L"HSpRenderer");
	m_ComListCombo.AddString(L"KCollider2D");

	UpdateData(FALSE);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void AddCom::OnBnClickedAddcom()
{
	UpdateData(TRUE);

	CString Name;

	m_ComListCombo.GetLBText(m_ComListCombo.GetCurSel(), Name);

	CreateCom(Name);

	Edit_Class::m_gVIewCom->Reset();

	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
