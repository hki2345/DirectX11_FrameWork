// Dlg_MeshConv.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_MeshConv.h"
#include "afxdialogex.h"


// Dlg_MeshConv 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_MeshConv, TabDlg)

Dlg_MeshConv::Dlg_MeshConv(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_MESHDLG, pParent)
{

}

Dlg_MeshConv::~Dlg_MeshConv()
{
}

void Dlg_MeshConv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_MeshConv, TabDlg)
END_MESSAGE_MAP()


// Dlg_MeshConv 메시지 처리기입니다.
