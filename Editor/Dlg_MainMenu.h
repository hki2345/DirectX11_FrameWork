#pragma once
#include "afxcmn.h"
#include <vector>
#include <State.h>
#include <Core_Class.h>
#include "TabDlg.h"


// Dlg_MainMenu ��ȭ �����Դϴ�.

class Dlg_MainMenu : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_MainMenu)

public:
	Dlg_MainMenu(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_MainMenu();

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


		// Dlg�� ���鋚 ������ �������� ��� ����
		// �ش� ���� ���� ��������� �ʴ´�. -> �������� ��ü�� ����.
		if (true == _IsSceneCreate)
		{
			KPtr<State> m_Scene = Core_Class::MainSceneMgr().Find_State(_ItemName.GetString());
			if (nullptr == m_Scene)
			{
				m_Scene = Core_Class::MainSceneMgr().Create_State(_ItemName.GetString());
				KPtr<TheOne> Cam = m_Scene->Create_One();
				KPtr<Camera> CamCom = Cam->Add_Component<Camera>();
				CamCom->Insert_LayerData(0);
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

	void Show_Dlg(size_t _Index);

public:
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);

};

