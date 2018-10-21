#pragma once
#include "ComDlg.h"
#include <HSpRenderer.h>
#include "afxcolorbutton.h"
#include "afxcmn.h"

// SpRenderDlg 대화 상자입니다.

class SpRenderDlg : public ComDlg
{
	DECLARE_DYNAMIC(SpRenderDlg)

public:
	SpRenderDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SpRenderDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRENDER };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	HSpRenderer* m_pRender;
	void SettingComponent(Component* _pCom) override 
	{
		m_pRender = (HSpRenderer*)_pCom;
	}
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSpritebutton();


	CMFCColorButton m_ColorCtr;
	afx_msg void OnNMCustomdrawUvslider(NMHDR *pNMHDR, LRESULT *pResult);

	CSliderCtrl m_UVSL;
	int m_UVSValue;
	int m_UVVALUE;
	afx_msg void OnEnChangeUvindex();
};
