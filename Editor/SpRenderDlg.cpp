// SpRenderDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "SpRenderDlg.h"
#include "afxdialogex.h"
#include "SpriteSelectDlg.h"


// SpRenderDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(SpRenderDlg, CDialogEx)

SpRenderDlg::SpRenderDlg(CWnd* pParent /*=NULL*/)
	: ComDlg(IDD_SPRENDER, pParent)
	, m_UVSValue(0)
	, m_UVVALUE(0)
{
	SIZEY = 180;
}

SpRenderDlg::~SpRenderDlg()
{
}

void SpRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_ColorCtr);
	DDX_Control(pDX, IDC_UVSLIDER, m_UVSL);

	DDX_Slider(pDX, IDC_UVSLIDER, m_UVSValue);
	DDX_Text(pDX, IDC_UVINDEX, m_UVVALUE);
}


BEGIN_MESSAGE_MAP(SpRenderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SPRITEBUTTON, &SpRenderDlg::OnBnClickedSpritebutton)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_UVSLIDER, &SpRenderDlg::OnNMCustomdrawUvslider)
	ON_EN_CHANGE(IDC_UVINDEX, &SpRenderDlg::OnEnChangeUvindex)
END_MESSAGE_MAP()


// SpRenderDlg �޽��� ó�����Դϴ�.


BOOL SpRenderDlg::OnInitDialog()
{
	ComDlg::OnInitDialog();

	m_UVSL.SetRange(0, 0);


	// m_ColorCtr.alpha

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void SpRenderDlg::OnBnClickedSpritebutton()
{
	SpriteSelectDlg Dlg;

	Dlg.DoModal();

	m_pRender->Image(Dlg.ImageName);
	m_pRender->ImageIndex(0);

	UpdateData(TRUE);

	m_UVSL.SetRange(0, (int)m_pRender->Image()->CutCount() - 1);

	UpdateData(FALSE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void SpRenderDlg::OnNMCustomdrawUvslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;


	pNMCD;

	UpdateData(TRUE);

	
	m_pRender->ImageIndex(m_UVSValue);
	m_UVVALUE = m_UVSValue;

	UpdateData(FALSE);
}



void SpRenderDlg::OnEnChangeUvindex()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// ComDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	UpdateData(TRUE);


	m_pRender->ImageIndex(m_UVVALUE);
	m_UVSValue = m_UVVALUE;

	UpdateData(FALSE);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
