#pragma once
#include "afxcmn.h"
#include <vector>
#include <State.h>
#include <Core_Class.h>

#include "EditUpdater.h"
#include "TabState.h"
// Dlg_Main_Menu 대화 상자입니다.

class Dlg_Main_Menu : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_Main_Menu)

public:
	Dlg_Main_Menu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Main_Menu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_MENU };
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
	size_t m_CurTab;

private:
	std::vector<TabState*> m_Dlgvec;
	template<typename T>
	void Create_Dialog(UINT _DId, CString _Name, bool _IsStateCreate = true)
	{
		UpdateData(TRUE);

		RECT newRc;
		TabMenu.GetClientRect(&newRc);
		TabMenu.InsertItem((int)m_Dlgvec.size(), _Name);


		// 해당 탭은 씬을 만들어주지 않는다. -> 업데이터 자체가 없다.
		if (true == _IsStateCreate)
		{
			KPtr<State> m_State = Core_Class::Main_StateManager().find_state(_Name.GetString());
			if (nullptr == m_State)
			{
				m_State = Core_Class::Main_StateManager().create_state(_Name);
				// m_State->this_KRigid2DManager.gravity({ .0f, -200.0f });
				// m_State->this_Collider2DManager.chain(0);

				KPtr<TheOne> newCamera = m_State->Create_TheOne();
				newCamera->Local_Pos(KVector(.0f, .0f, -10.0f, .0f));

				KPtr<Camera> CameCom = newCamera->Add_Component<Camera>();
				CameCom->Insert_LayerData(0, 1, 2, 3, 4);
			}
		}

		T* m_DLG = new T();

		m_DLG->StateName = _Name;
		m_DLG->Create(_DId, this);
		m_DLG->SetBackgroundColor(RGB(255, 255, 255), TRUE);
		m_DLG->SetWindowPos(nullptr, 36, 18, newRc.right - 36, newRc.bottom - 18, SWP_NOZORDER);
		m_DLG->ShowWindow(SW_HIDE);
		m_DLG->m_StateChange = _IsStateCreate;

		m_Dlgvec.push_back(m_DLG);



		UpdateData(FALSE);
	}

	void show(size_t _Inx);

public:
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void Init();
};
