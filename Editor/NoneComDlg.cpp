// NoneComDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "NoneComDlg.h"
#include "afxdialogex.h"

// NoneComDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(NoneComDlg, CDialogEx)

NoneComDlg::NoneComDlg(CWnd* pParent /*=NULL*/)
	: ComDlg(IDD_NONECOM, pParent)
{
	SIZEY = 30;
}

NoneComDlg::~NoneComDlg()
{
}

void NoneComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NoneComDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NoneComDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &NoneComDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// NoneComDlg �޽��� ó�����Դϴ�.


void NoneComDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void NoneComDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
