// Dlg_Render_Sprite.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_Render_Sprite.h"
#include "afxdialogex.h"
#include "Dlg_SpriteSelect.h"


// Dlg_Render_Sprite ��ȭ �����Դϴ�.

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


// Dlg_Render_Sprite �޽��� ó�����Դϴ�.

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

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void Dlg_Render_Sprite::OnNMCustomdrawUvslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// Dlg_Component::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	UpdateData(TRUE);

	m_Renderer->index(m_UVBoxValue);
	m_UVSliderValue = m_UVBoxValue;

	UpdateData(FALSE);
}
