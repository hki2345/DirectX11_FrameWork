// Dlg_UnitList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_UnitList.h"
#include "afxdialogex.h"


// Dlg_UnitList 대화 상자입니다.

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


// Dlg_UnitList 메시지 처리기입니다.




void Dlg_UnitList::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void Dlg_UnitList::OnLbnDblclkList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
