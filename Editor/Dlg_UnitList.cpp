// Dlg_UnitList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Dlg_UnitList.h"
#include "afxdialogex.h"


// Dlg_UnitList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_UnitList, CDialogEx)

Dlg_UnitList::Dlg_UnitList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNITLISTDLG, pParent)
{

}

Dlg_UnitList::~Dlg_UnitList()
{
}

void Dlg_UnitList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_UnitList, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dlg_UnitList::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST1, &Dlg_UnitList::OnLbnDblclkList1)
END_MESSAGE_MAP()


// Dlg_UnitList �޽��� ó�����Դϴ�.




void Dlg_UnitList::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}


void Dlg_UnitList::OnLbnDblclkList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
