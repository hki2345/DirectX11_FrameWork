// Dlg_Render_Sprite.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_Render_Sprite.h"
#include "afxdialogex.h"
#include "Dlg_SpriteSelect.h"


// Dlg_Render_Sprite 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Render_Sprite, CDialogEx)

Dlg_Render_Sprite::Dlg_Render_Sprite(CWnd* pParent /*=NULL*/)
	: Dlg_Component(IDD_DLG_SPRITERENDER, pParent)
	, m_UVSliderValue(0)
	, m_UVBoxValue(0)
{
	Y_SIZE = 180;
}

Dlg_Render_Sprite::~Dlg_Render_Sprite()
{
}

void Dlg_Render_Sprite::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPRITE_COLORBTN, m_ColorBtn);
	DDX_Control(pDX, IDC_UVSLIDER, m_UVSlider);

	DDX_Slider(pDX, IDC_UVSLIDER, m_UVSliderValue);
	DDX_Text(pDX, IDC_UVINDEX, m_UVBoxValue);
}


BEGIN_MESSAGE_MAP(Dlg_Render_Sprite, CDialogEx)
	ON_BN_CLICKED(IDC_IMAGE_SLT, &Dlg_Render_Sprite::OnBnClickedImageSlt)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_UVSLIDER, &Dlg_Render_Sprite::OnNMCustomdrawUvslider)
	ON_EN_CHANGE(IDC_UVINDEX, &Dlg_Render_Sprite::OnEnChangeUvindex)
END_MESSAGE_MAP()


// Dlg_Render_Sprite 메시지 처리기입니다.

BOOL Dlg_Render_Sprite::OnInitDialog()
{
	Dlg_Component::OnInitDialog();

	m_UVSlider.SetRange(0, 0);

	return TRUE;
}


void Dlg_Render_Sprite::OnBnClickedImageSlt()
{
	Dlg_SpriteSelect Dlg;

	Dlg.DoModal();

	m_Renderer->image(Dlg.Image_Name + ".png");
	m_Renderer->index(0);

	UpdateData(TRUE);
	
	m_UVSlider.SetRange(0, (int)m_Renderer->image()->split_cnt() - 1);

	UpdateData(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Dlg_Render_Sprite::OnNMCustomdrawUvslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (nullptr == m_Renderer)
	{
		return;
	}

	UpdateData(TRUE);

	m_Renderer->index(m_UVSliderValue);
	m_UVBoxValue = m_UVSliderValue;

	UpdateData(FALSE);
}


void Dlg_Render_Sprite::OnEnChangeUvindex()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// Dlg_Component::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.


	UpdateData(TRUE);

	m_Renderer->index(m_UVBoxValue);
	m_UVSliderValue = m_UVBoxValue;

	UpdateData(FALSE);
}
