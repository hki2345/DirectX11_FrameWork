#pragma once
#include "TabDlg.h"
#include "afxwin.h"



// Dlg_Terrain 대화 상자입니다.

class Renderer_Terrain;
class Dlg_Terrain : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Terrain)

public:
	Dlg_Terrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Terrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINDLG };
#endif

private:
	KPtr<Renderer_Terrain> m_pTer;
	CEdit NameEdit[3];
	float TerSSEdit[4];
	float UnitPosEdit[3];
	CEdit TexName;
	CListBox UList;


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public:
	void Init_Dlg() override;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedTerclear();
	afx_msg void OnBnClickedTerload();
	afx_msg void OnBnClickedTersave();
	afx_msg void OnBnClickedTercovertex();
	afx_msg void OnBnClickedStateclear();
	afx_msg void OnBnClickedStateload();
	afx_msg void OnBnClickedStatesave();


	afx_msg void OnEditSelChanged(UINT _Id);
	afx_msg void OnTerInfoSelChanged(UINT _Id);
	afx_msg void OnUnitPosSelChanged(UINT _Id);
	afx_msg void OnBnClickedTereditbtn();
	afx_msg void OnBnClickedStatereslist();
};
