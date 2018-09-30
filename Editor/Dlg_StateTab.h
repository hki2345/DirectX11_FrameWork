#pragma once
#include "afxcmn.h"
#include "TabState.h"
#include <map>


// Dlg_SceneTab ��ȭ �����Դϴ�.

class Dlg_StateTab : public TabState
{
	DECLARE_DYNAMIC(Dlg_StateTab)

public:
	Dlg_StateTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_StateTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCENE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


	DECLARE_MESSAGE_MAP()
public:
	CString m_State_Name;
	CString m_CurState_Name;
	CListBox m_State_List;
	void Init();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedScenecreator();
	afx_msg void OnLbnSelchangeScenelist();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
