#pragma once
#include "Dlg_Component.h"
#include <Renderer_Sprite.h>
#include "afxcolorbutton.h"


// Dlg_Render_Sprite ��ȭ �����Դϴ�.

class Dlg_Render_Sprite : public Dlg_Component
{
	DECLARE_DYNAMIC(Dlg_Render_Sprite)

public:
	Dlg_Render_Sprite(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Render_Sprite();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SPRITERENDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	Renderer_Sprite* m_Renderer;

public:
	void set_component(Component* _Com) override
	{
		// �ٿ� ĳ���� -> Ȯ���� ��쿡�� �ռ�
		m_Renderer = (Renderer_Sprite*)_Com;
	}
	
	CMFCColorButton m_ColorBtn;
	CSliderCtrl m_UVSlider;

	int m_UVSliderValue;
	int m_UVBoxValue;


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedImageSlt();
	afx_msg void OnNMCustomdrawUvslider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeUvindex();
};
