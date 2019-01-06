#pragma once
#include "TabDlg.h"
#include "afxwin.h"



// Dlg_Terrain ��ȭ �����Դϴ�.

class Renderer_Terrain;
class SC2_Force;
class SC2_Camera;
class Force_Unit;
class Dlg_Terrain : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Terrain)

public:
	Dlg_Terrain(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Terrain();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINDLG };
#endif

private:
	bool m_bGrab;

	KPtr<SC2_Camera>				m_pCam;
	KPtr<Renderer_Terrain>			m_pTer;
	KPtr<SC2_Force>					m_Force;
	KPtr<Force_Unit>				m_GrabUnit;
	KPtr<Force_Unit>				m_SelectUnit;

	KPtr<Force_Unit>				m_CurPlayer;

	std::vector<KPtr<Force_Unit>>	m_UComVec;

	CEdit NameEdit[3];
	float TerSSEdit[4];
	float UnitPosEdit[3];
	CEdit TexName;
	CListBox UBoxList;
	CButton m_TerBtn;


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	KPtr<Force_Unit> Create_Unit();

	void Update_SSPos();


	void Update_Force();
	void Update_UnitList();
	void Update_Terrain();
	void Update_Grab();
	void Udpate_Delete();
	void Update_Col();

	void Update_StayCol(KCollision* _Left, KCollision* _Right);

public:
	void Init_Dlg() override;
	void Update_Dlg() override;

	void Grab_On()
	{
		m_bGrab = true;
	}

	void Grab_Off()
	{
		m_bGrab = false;
	}

	void Create_Grab(const wchar_t* _Name);

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
	afx_msg void OnBnClickedStatereslist();
	afx_msg void OnBnClickedTereditbtn();
	afx_msg void OnBnClickedTersetplay();
};
