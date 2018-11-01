// AddCom.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "AddCom.h"
#include "afxdialogex.h"
#include "Edit_Class.h"


// AddCom ��ȭ �����Դϴ�.

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


// AddCom �޽��� ó�����Դϴ�.


BOOL AddCom::OnInitDialog()
{
	ComDlg::OnInitDialog();

	UpdateData(TRUE);

	m_ComListCombo.AddString(L"HSpRenderer");
	m_ComListCombo.AddString(L"KCollider2D");

	UpdateData(FALSE);

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void AddCom::OnBnClickedAddcom()
{
	UpdateData(TRUE);

	CString Name;

	m_ComListCombo.GetLBText(m_ComListCombo.GetCurSel(), Name);

	CreateCom(Name);

	Edit_Class::m_gVIewCom->Reset();

	UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
