// NoneComDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "NoneComDlg.h"
#include "afxdialogex.h"

// NoneComDlg 대화 상자입니다.

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


// NoneComDlg 메시지 처리기입니다.


void NoneComDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void NoneComDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
