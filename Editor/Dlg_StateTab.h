#pragma once
#include "afxcmn.h"
#include "TabState.h"
#include <map>


// Dlg_SceneTab 대화 상자입니다.

class Dlg_StateTab : public TabState
{
	DECLARE_DYNAMIC(Dlg_StateTab)

public:
	Dlg_StateTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_StateTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCENE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


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
