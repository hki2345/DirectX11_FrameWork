// Dlg_NoneCom.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_NoneCom.h"
#include "afxdialogex.h"


// Dlg_NoneCom ��ȭ �����Դϴ�.

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


// Dlg_NoneCom �޽��� ó�����Դϴ�.
