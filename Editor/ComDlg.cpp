// ComDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "ComDlg.h"


// ComDlg ��ȭ �����Դϴ�.



BOOL ComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
BEGIN_MESSAGE_MAP(ComDlg, CDialogEx)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


void ComDlg::OnNcPaint()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnNcPaint()��(��) ȣ������ ���ʽÿ�.

}
