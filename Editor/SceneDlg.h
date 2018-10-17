#pragma once
#include "afxwin.h"
#include "TabDlg.h"

// SceneDlg 대화 상자입니다.

class SceneDlg : public TabDlg
{
	DECLARE_DYNAMIC(SceneDlg)

public:
	SceneDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SceneDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCENEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCrescene();
	afx_msg void OnLbnSelchangeScenelist();

public:
	void Init();

public:
	CString m_SceneName;
	CString m_CurSceneName;
	CListBox m_SceneList;
	// afx_msg void OnBnClickedCreateobj();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedSavescene();
};
