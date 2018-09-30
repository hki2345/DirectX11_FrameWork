// Dlg_NoneCom.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_NoneCom.h"
#include "afxdialogex.h"


// Dlg_NoneCom 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_NoneCom, Dlg_Component)

Dlg_NoneCom::Dlg_NoneCom(CWnd* pParent /*=NULL*/)
	: Dlg_Component(IDD_DLG_NONECOM, pParent)
{
	Y_SIZE = 30;
}

Dlg_NoneCom::~Dlg_NoneCom()
{
}

void Dlg_NoneCom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_NoneCom, CDialogEx)
END_MESSAGE_MAP()


// Dlg_NoneCom 메시지 처리기입니다.
