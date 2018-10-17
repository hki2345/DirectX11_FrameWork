// SpRenderDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "SpRenderDlg.h"
#include "afxdialogex.h"
#include "SpriteSelectDlg.h"


// SpRenderDlg 대화 상자입니다.

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


// SpRenderDlg 메시지 처리기입니다.


BOOL SpRenderDlg::OnInitDialog()
{
	ComDlg::OnInitDialog();

	m_UVSL.SetRange(0, 0);


	// m_ColorCtr.alpha

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void SpRenderDlg::OnNMCustomdrawUvslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	pNMCD;

	UpdateData(TRUE);

	
	m_pRender->ImageIndex(m_UVSValue);
	m_UVVALUE = m_UVSValue;

	UpdateData(FALSE);
}



void SpRenderDlg::OnEnChangeUvindex()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// ComDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(TRUE);


	m_pRender->ImageIndex(m_UVVALUE);
	m_UVSValue = m_UVVALUE;

	UpdateData(FALSE);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
