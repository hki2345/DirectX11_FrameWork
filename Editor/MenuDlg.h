#pragma once
#include "afxcmn.h"
#include <vector>
#include <State.h>
#include <Core_Class.h>
#include "TabDlg.h"


// MenuDlg 대화 상자입니다.

class MenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MenuDlg)

public:
	MenuDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MenuDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENUDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
			KPtr<State> m_Scene = Core_Class::MainSceneMgr().Find_State(_ItemName.GetString());
			if (nullptr == m_Scene)
			{
				m_Scene = Core_Class::MainSceneMgr().Create_State(_ItemName.GetString());
				KPtr<TheOne> Cam = m_Scene->Create_One();
				Cam->Trans()->pos_local(KVector4(0.0f, 0.0f, -10.0f));
				KPtr<Camera> CamCom = Cam->Add_Component<Camera>();
				CamCom->Insert_LayerData(0, 1, 2, 3, 4);
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

