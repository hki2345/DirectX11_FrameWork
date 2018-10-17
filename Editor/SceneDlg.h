#pragma once
#include "afxwin.h"
#include "TabDlg.h"

// SceneDlg ��ȭ �����Դϴ�.

class SceneDlg : public TabDlg
{
	DECLARE_DYNAMIC(SceneDlg)

public:
	SceneDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~SceneDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCENEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
