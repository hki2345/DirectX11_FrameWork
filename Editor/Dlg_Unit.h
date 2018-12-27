#pragma once
#include "TabDlg.h"
#include "afxwin.h"



// Dlg_Unit 대화 상자입니다.

class Renderer_BonAni;
class Force_Unit;
class Dlg_Unit : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Unit)

public:
	Dlg_Unit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Unit();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITDLG };
#endif

protected:
	void Update_RscTree();
	void Update_AddModel();
	void Update_DelModel();
	void Update_SelectList();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_KM3List;
	CListBox m_SelectList;

	KPtr<Renderer_Terrain>				m_pTer;
	KPtr<SC2_Camera>					m_pCam;
	KPtr<TheOne>						m_CurOne;
	KPtr<Force_Unit>					m_CurUnit;


	std::wstring						m_CurStr;

	std::list<KPtr<Renderer_BonAni>>	m_RenderList;
	std::list<KPtr<Renderer_BonAni>>::iterator m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator m_ERI;


	CEdit InfoValue[6];
	CButton WeaponType[3];


	virtual BOOL OnInitDialog();
	
	void Init_Dlg() override;
	void Update_Dlg() override;

	afx_msg void OnBnClickedReninsertbtn();
	afx_msg void OnBnClickedRendeletebtn();
	afx_msg void OnBnClickedUnitloadbtn();
	afx_msg void OnBnClickedUnitsavebtn();
	afx_msg void OnLbnSelchangeUnitkm3list();
	afx_msg void OnBnClickedRenrestartbtn();
	afx_msg void UnitInfoSelchange(UINT _Id);
	afx_msg void UnitWeaponSelchange(UINT _Id);
};
