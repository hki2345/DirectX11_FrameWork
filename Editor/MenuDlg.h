#pragma once
#include "afxcmn.h"
#include <vector>
#include <HScene.h>
#include <HVAR.h>
#include "TabDlg.h"


// MenuDlg ��ȭ �����Դϴ�.

class MenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MenuDlg)

public:
	MenuDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MenuDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENUDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

public:
	CTabCtrl TabMenu;

	size_t m_CurIndex;

	//SceneDlg m_SceneDlg;
	//SpriteDlg m_SpriteDlg;

private:
	std::vector<TabDlg*> m_VecDlg;

	template<typename T>
	void CreateDlg(UINT _DlgId, CString _ItemName, bool _IsSceneCreate = true) 
	{
		UpdateData(TRUE);

		RECT Rc;
		TabMenu.GetClientRect(&Rc);
		TabMenu.InsertItem((int)m_VecDlg.size(), _ItemName);


		if (true == _IsSceneCreate)
		{
			HPTR<HScene> m_Scene = HVAR::MainSceneMgr().FindScene(_ItemName.GetString());
			if (nullptr == m_Scene)
			{
				m_Scene = HVAR::MainSceneMgr().CreateScene(_ItemName.GetString());
				HPTR<HActor> Camera = m_Scene->CreateActor();
				Camera->Trans()->LPos(HVEC(0.0f, 0.0f, -10.0f));
				HPTR<HCamera> CamCom = Camera->AddCom<HCamera>();
				CamCom->PushRenderLayer(0, 1, 2, 3, 4);
			}
		}

		T* m_pDlg = new T();

		m_pDlg->SceneName = _ItemName;
		m_pDlg->Create(_DlgId, this);
		m_pDlg->SetBackgroundColor(RGB(255, 255, 255), TRUE);
		m_pDlg->SetWindowPos(nullptr, 36, 18, Rc.right - 36, Rc.bottom - 18, SWP_NOZORDER);
		m_pDlg->ShowWindow(SW_HIDE);
		m_pDlg->m_SceneChange = _IsSceneCreate;

		m_VecDlg.push_back(m_pDlg);

		UpdateData(FALSE);
	}

	void ShowDlg(size_t _Index);

public:
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);

};
