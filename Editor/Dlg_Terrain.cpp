// Dlg_Terrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_Terrain.h"
#include "afxdialogex.h"

#include "Dlg_UnitList.h"
#include "Edit_Class.h"
#include <Con_Class.h>


#include <Core_Class.h>
#include <SC2_Camera.h>
#include <SC2_Force.h>

#include <Controll_AI.h>
#include <Controll_User.h>

#include <Force_Unit.h>

#include <Light.h>
#include <Texture_Multi.h>


#include <Renderer_Mesh.h>
#include <Renderer_Grid.h>
#include <Renderer_Terrain.h>

#include <Renderer_BonAni.h>
#include <ResourceManager.h>
#include <InputManager.h>

#include <KRay3D.h>
#include <KBox_Col.h>

// Dlg_Terrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Terrain, TabDlg)

Dlg_Terrain::Dlg_Terrain(CWnd* pParent /*=NULL*/) :
	TabDlg(IDD_TERRAINDLG, pParent),
	m_Force(nullptr)
{

}

Dlg_Terrain::~Dlg_Terrain()
{
	m_Force->Clear_Unit();
	m_UComVec.clear();
}

BOOL Dlg_Terrain::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(StateName.GetString());

	if (nullptr == TabScene)
	{
		KASSERT(true);
	}

	// 투명한 걸 그린다면 만약에 -> 스카이랑 섞이기 위해선 당연히 ... 스카이가 디퍼드로 그려져야 한다.
	KPtr<TheOne> SKYMESH = TabScene->Create_One();
	SKYMESH->Trans()->scale_local(KVector4(10000.0f, 10000.0f, 10000.0f));
	SKYMESH->Trans()->pos_local(KVector4(.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> SKYMESH1 = SKYMESH->Add_Component<Renderer_Mesh>();
	SKYMESH1->Set_Material(L"SKY3DMATFORWARD");
	SKYMESH1->Set_RSState(L"SNONE");
	SKYMESH1->ROpt.Defferd_orForward = 0;
	SKYMESH1->ROpt.LightOpacity = 1.0f;
	SKYMESH1->Set_Mesh(L"SPHERE");
	SKYMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"Space.jpg");

	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	KPtr<Renderer_Grid> GRIDRENDER = GRIDACTOR->Add_Component<Renderer_Grid>();


	m_pCam = TabScene->Camera()->Add_Component<SC2_Camera>();
	TabScene->Camera()->Far(10000.0f);
	TabScene->Camera()->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));




	KPtr<TheOne> Light2 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight2 = Light2->Add_Component<Light>();
	pLight2->Trans()->rotate_world(KVector4(45.0F, 0.0F, 0.0f));
	pLight2->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight2->PushLightLayer(0);


	KPtr<TheOne> Light3 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight3 = Light3->Add_Component<Light>();
	pLight3->Trans()->rotate_world(KVector4(-45.0F, 0.0F, 0.0f));
	pLight3->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight3->PushLightLayer(1);


	KPtr<TheOne> Light4 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight4 = Light4->Add_Component<Light>();
	pLight4->Trans()->rotate_world(KVector4(.0F, 45.0F, 0.0f));
	pLight4->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight4->PushLightLayer(0);

	KPtr<TheOne> TERRAIN = TabScene->Create_One();
	TERRAIN->Trans()->scale_local(KVector4(5.0f, 2.0f, 5.0f));
	TERRAIN->Trans()->pos_world(KVector4(5.0f, .0f, .0f, .0f));
	m_pTer = TERRAIN->Add_Component<Renderer_Terrain>();

	 // 순서를 지켜야 된다????? ㅇㅇ
	m_pTer->Create_Terrain(64, 64, L"Cover.jpg", 1.0f);
	m_pTer->base_texture(L"FB");
	m_pTer->Insert_CoverTex(L"FC", L"Cover.jpg");
	m_pTer->Set_RSState(L"SFRONT");
	m_pTer->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 7, L"cursor-target-allied.dds");
	m_pTer->brush_size(10.0f);



	TabScene->This_Col3DManager.Link(100, 100);

	KPtr<KRay3D> RayCol = TabScene->Camera()->Add_Component<KRay3D>(100);
	RayCol->StayFunc(this, &Dlg_Terrain::Update_StayCol);


	if (nullptr != Con_Class::s2_manager())
	{
		m_Force = Con_Class::s2_manager()->Find_Force(L"TT");
	}
	else
	{
		KPtr<SC2Manager> MManager = new SC2Manager();
		Con_Class::s2_manager(MManager);
	}


	// PathManager::Is_StrVSStr
	UpdateData(TRUE);
	TerSSEdit[0] = (float)m_pTer->split_X();
	TerSSEdit[1] = (float)m_pTer->split_Z();

	TerSSEdit[2] = m_pTer->one()->Trans()->scale_local().x;
	TerSSEdit[3] = m_pTer->one()->Trans()->scale_local().x;

	for (size_t i = 0; i < 3; i++)
	{
		UnitPosEdit[i] = .0f;
	}
	UpdateData(FALSE);

	return TRUE;
}

void Dlg_Terrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TERTEXPATH, TexName);


	int StartId = IDC_TERNAME;

	// 스피드 * 2, 이름, 스케일
	for (size_t X = 0; X < 3; ++X)
	{
		DDX_Control(pDX, StartId, NameEdit[X]);
		++StartId;
	}



	StartId = IDC_TERSPLITX;
	// 조각 X Z, 스케일 X Z
	for (size_t X = 0; X < 4; ++X)
	{
		DDX_Text(pDX, StartId, TerSSEdit[X]);
		++StartId;
	}


	StartId = IDC_UNITPOSXEDIT;
	// 조각 X Z, 스케일 X Z
	for (size_t X = 0; X < 3; ++X)
	{
		DDX_Text(pDX, StartId, UnitPosEdit[X]);
		++StartId;
	}

	StartId = IDC_UNITSTATICPOSX;
	// 조각 X Z, 스케일 X Z
	for (size_t X = 0; X < 3; ++X)
	{

		m_PosDrop[X].pValue = &UnitPosEdit[X];
		m_PosDrop[X].Parent = this;
		m_PosDrop[X].ValueChangeFunc(this, &Dlg_Terrain::Update_SSPosFunc);
		DDX_Control(pDX, StartId, m_PosDrop[X]);



		switch (X)
		{
		case 0:
			m_PosDrop[X].SetWindowTextW(L"X");
			break;
		case 1:
			m_PosDrop[X].SetWindowTextW(L"Y");
			break;
		case 2:
			m_PosDrop[X].SetWindowTextW(L"Z");
			break;
		default:
			break;
		}
		++StartId;
	}

	DDX_Control(pDX, IDC_TERUNITLIST, UBoxList);
	DDX_Control(pDX, IDC_TEREDITBTN, m_TerBtn);
}


void Dlg_Terrain::Init_Dlg()
{
}

void Dlg_Terrain::Update_SSPos()
{
	int A = UBoxList.GetCurSel();
	if (0 > A)
	{
		return;
	}

	if (nullptr != m_UComVec[A])
	{
		KVector TVec = m_UComVec[A]->one()->Trans()->pos_local();

		UpdateData(TRUE);
		UnitPosEdit[0] = TVec.m1;
		UnitPosEdit[1] = TVec.m2;
		UnitPosEdit[2] = TVec.m3;
		UpdateData(FALSE);
	}
}

void Dlg_Terrain::Update_SSPosFunc()
{
	if (nullptr == m_SelectUnit)
	{
		int A = UBoxList.GetCurSel();

		if (0 <= A)
		{
			m_SelectUnit = m_UComVec[A];
		}
		else
		{
			return;
		}
	}

	KVector TVec = KVector(UnitPosEdit[0], UnitPosEdit[1], UnitPosEdit[2]);

	float TMP = m_pTer->Y_Terrain(TVec);
	TVec.y = TMP;
	m_SelectUnit->one()->Trans()->pos_local(TVec);
}

void Dlg_Terrain::Update_Force()
{
	m_UComVec.clear();

	if (nullptr == m_Force)
	{
		return;
	}
	
	std::list<KPtr<Force_Unit>>* TT = m_Force->unit_list();
	std::list<KPtr<Force_Unit>>::iterator S = TT->begin();
	std::list<KPtr<Force_Unit>>::iterator E = TT->end();

	for (; S != E; ++S)
	{
		m_UComVec.push_back((*S));
	}
}

void Dlg_Terrain::Update_UnitList()
{
	UBoxList.ResetContent();

	for (size_t i = 0; i < m_UComVec.size(); i++)
	{
		if (true == m_UComVec[i]->one()->Is_Active())
		{
			UBoxList.AddString(m_UComVec[i]->name());
		}
	}
}

void Dlg_Terrain::Update_Dlg()
{
	if (true == KEY_DOWN(L"INGAME"))
	{
		if (SC2_Camera::SC2_CAMMODE::S2M_EDIT == m_pCam->cam_mode())
		{
			m_pCam->Set_InGame();
		}
		else if (SC2_Camera::SC2_CAMMODE::S2M_INGAME == m_pCam->cam_mode())
		{
			m_pCam->Set_Edit();
		}
	}


	Update_Terrain();
	Update_Grab();
	Udpate_Delete();
	Update_SSPos();
	Update_Col();
}

void Dlg_Terrain::Update_Terrain()
{
	if (true == KEY_DOWN(L"ESC"))
	{
		m_pTer->Edit_Off();
		m_TerBtn.EnableWindow(TRUE);
	}
}

void Dlg_Terrain::Update_Grab()
{
	if (true == m_bGrab)
	{
		if (true == KEY_DOWN(L"RB"))
		{
			m_pTer->Mouse_CalOff();
			m_bGrab = false;
			m_GrabUnit->one()->Set_Death();
			m_GrabUnit = nullptr;
		}

		else if (true == KEY_DOWN(L"LB"))
		{
			if (true == m_pTer->Is_OnTer())
			{
				m_UComVec.push_back(Create_Unit());
			}

			Update_UnitList();
		}

		if (nullptr != m_GrabUnit)
		{
			m_pTer->Mouse_CalOn();
			m_GrabUnit->one()->Trans()->pos_local(m_pTer->pos_mouse());

			if (true == m_pTer->Is_Edit())
			{
				m_pTer->Edit_Off();
			}
		}
	}
}

void Dlg_Terrain::Udpate_Delete()
{
	if (true == KEY_DOWN(L"DEL"))
	{
		int A = UBoxList.GetCurSel();
		if (0 > A)
		{
			return;
		}

		if (nullptr != m_UComVec[A])
		{
			std::vector<KPtr<Force_Unit>>::iterator Iter = m_UComVec.begin();

			for (size_t i = 0; i < m_UComVec.size(); ++i, ++Iter)
			{
				if (m_UComVec[A] == m_UComVec[i])
				{
					break;
				}
			}

			m_Force->Delete_Unit((*Iter));
			m_UComVec.erase(Iter);
		}
		Update_UnitList();
	}
}

void Dlg_Terrain::Update_Col()
{
	int A = UBoxList.GetCurSel();
	if (0 > A)
	{
		return;
	}

	for (size_t i = 0; i < m_UComVec.size(); i++)
	{
		if (m_UComVec[i] == m_CurPlayer)
		{
			continue;
		}
		m_UComVec[i]->Get_Component<KBox_Col>()->debug_color(KColor::Green);
	}


	if (nullptr != m_UComVec[A] && m_UComVec[A] != m_CurPlayer)
	{
		m_UComVec[A]->Get_Component<KBox_Col>()->debug_color(KColor::Yellow);
	}
}


KPtr<Force_Unit> Dlg_Terrain::Create_Unit()
{
	if (nullptr == m_GrabUnit)
	{
		return nullptr;
	}

	KPtr<Force_Unit> TOne = m_Force->Create_Unit(m_GrabUnit->name());
	TOne->one()->Trans()->pos_local(m_GrabUnit->one()->Trans()->pos_local());

	return TOne;
}

void Dlg_Terrain::Create_Grab(const wchar_t* _Name)
{
	if (nullptr != m_GrabUnit)
	{
		m_GrabUnit->one()->Set_Death();
	}

	KPtr<State> TabScene = Core_Class::MainScene();
	KPtr<TheOne> TOne = TabScene->Create_One();

	TOne->Trans()->pos_local(KVector(.0f));
	TOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	m_GrabUnit = TOne->Add_Component<Force_Unit>(_Name);

	m_bGrab = true;
}



BEGIN_MESSAGE_MAP(Dlg_Terrain, TabDlg)
	ON_BN_CLICKED(IDC_TERCLEAR, &Dlg_Terrain::OnBnClickedTerclear)
	ON_BN_CLICKED(IDC_TERLOAD, &Dlg_Terrain::OnBnClickedTerload)
	ON_BN_CLICKED(IDC_TERSAVE, &Dlg_Terrain::OnBnClickedTersave)
	ON_BN_CLICKED(IDC_TERCOVERTEX, &Dlg_Terrain::OnBnClickedTercovertex)
	ON_BN_CLICKED(IDC_STATECLEAR, &Dlg_Terrain::OnBnClickedStateclear)
	ON_BN_CLICKED(IDC_STATELOAD, &Dlg_Terrain::OnBnClickedStateload)
	ON_BN_CLICKED(IDC_STATESAVE, &Dlg_Terrain::OnBnClickedStatesave)


	ON_CONTROL_RANGE(EN_CHANGE, IDC_TERNAME, IDC_STATETERNAME, &Dlg_Terrain::OnEditSelChanged)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_TERSPLITX, IDC_TERSCALEZ, &Dlg_Terrain::OnTerInfoSelChanged)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_UNITPOSXEDIT, IDC_UNITPOSZEDIT, &Dlg_Terrain::OnUnitPosSelChanged)
	ON_BN_CLICKED(IDC_STATERESLIST, &Dlg_Terrain::OnBnClickedStatereslist)
	ON_BN_CLICKED(IDC_TEREDITBTN, &Dlg_Terrain::OnBnClickedTereditbtn)
	ON_BN_CLICKED(IDC_TERSETPLAY, &Dlg_Terrain::OnBnClickedTersetplay)
END_MESSAGE_MAP()


// Dlg_Terrain 메시지 처리기입니다.


void Dlg_Terrain::OnBnClickedTerclear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pTer->Clear();
}


void Dlg_Terrain::OnBnClickedTerload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString TT;
	NameEdit[0].GetWindowTextW(TT);

	m_pTer->Load(TT.GetString());
}


void Dlg_Terrain::OnBnClickedTersave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pTer->Save();
}


void Dlg_Terrain::OnBnClickedTercovertex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Dlg_Terrain::OnBnClickedStateclear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Force->Clear_Unit();
	m_UComVec.clear();
	Update_UnitList();
}


void Dlg_Terrain::OnBnClickedStateload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Tem;
	NameEdit[1].GetWindowTextW(Tem);
	m_Force->Load(Tem.GetString());

	Update_Force();
	Update_UnitList();
}


void Dlg_Terrain::OnBnClickedStatesave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Force->Save();
}



void Dlg_Terrain::OnEditSelChanged(UINT _Id)
{
	UINT TempId = _Id - IDC_TERNAME;

	switch (TempId)
	{
	case 0:
	{
		CString TT;
		NameEdit[0].GetWindowTextW(TT);
		m_pTer->name(TT.GetString());
	}
		break;
	case 1:
	{
	}
		break;
	case 2:
	{
	}
		break;
	default:
		break;
	}
}

void Dlg_Terrain::OnTerInfoSelChanged(UINT _Id)
{
	UINT TempId = _Id - IDC_TERSPLITX;

	UpdateData(TRUE);
	UpdateData(FALSE);


	switch (TempId)
	{
	case 0:
	{
		m_pTer->split_X((int)TerSSEdit[0]);
	}
	break;
	case 1:
	{
		m_pTer->split_Z((int)TerSSEdit[1]);
	}
	break;
	case 2:
	{
		KVector TmpS = m_pTer->one()->Trans()->scale_local();
		TmpS.x = TerSSEdit[2];
		m_pTer->one()->Trans()->scale_local(TmpS);
	}
	break;
	case 3:
	{
		KVector TmpS = m_pTer->one()->Trans()->scale_local();
		TmpS.z = TerSSEdit[3];
		m_pTer->one()->Trans()->scale_local(TmpS);
	}
	break;
	default:
		break;
	}
}


void Dlg_Terrain::OnUnitPosSelChanged(UINT _Id)
{
	UINT TempId = _Id - IDC_UNITPOSXEDIT;
	UpdateData(TRUE);
	UpdateData(FALSE);


	int A = UBoxList.GetCurSel();
	if (0 > A)
	{
		return;
	}


	if (nullptr != m_UComVec[A])
	{
		KVector TVec = KVector(UnitPosEdit[0], UnitPosEdit[1], UnitPosEdit[2]);

		float TMP = m_pTer->Y_Terrain(TVec);
		TVec.y = TMP;
		m_UComVec[A]->one()->Trans()->pos_local(TVec);
	}
}

void Dlg_Terrain::OnBnClickedStatereslist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Dlg_UnitList* TT = (Dlg_UnitList*)Edit_Class::MDlg->Find_Dlg(L"Unit List");
	if (nullptr != TT)
	{
		TT->Init_Dlg(this);
		TT->ShowWindow(SW_SHOW);
	}
}


void Dlg_Terrain::OnBnClickedTereditbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pTer->Edit_On();
	m_TerBtn.EnableWindow(FALSE);
}


void Dlg_Terrain::OnBnClickedTersetplay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	
	// 나중에 충돌체로 얻을 수 있음
	if (nullptr == m_SelectUnit)
	{
		int A = UBoxList.GetCurSel();
		m_SelectUnit = m_UComVec[A];
	}
	

	if (nullptr == m_SelectUnit)
	{
		return;
	}

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(StateName.GetString());



	if (nullptr != m_SelectUnit->Get_Component<Controll_User>())
	{
		return;
	}
	
	m_SelectUnit->Delete_Component<Controll_AI>();
	KPtr<Controll_User> Cont = m_SelectUnit->Add_Component<Controll_User>(m_pTer, m_SelectUnit, m_pCam);
	Cont->Set_Render();


	if (nullptr != m_CurPlayer)
	{
		m_CurPlayer->Delete_Component<Controll_User>();
	}


	m_CurPlayer = m_SelectUnit;
	m_CurPlayer->Get_Component<KBox_Col>()->debug_color(KColor::Red);


	m_SelectUnit = nullptr;
}



void Dlg_Terrain::Update_StayCol(KCollision* _Left, KCollision* _Right)
{
	KPtr<KBox_Col> Tmp = _Left->Get_Component<KBox_Col>();

	if (nullptr == Tmp)
	{
		Tmp = _Right->Get_Component<KBox_Col>();
		if (nullptr == Tmp)
		{
			return;
		}
	}

	if (KEY_DOWN(L"LB"))
	{
		m_SelectUnit = Tmp->Get_Component<Force_Unit>();
		for (int i = 0; i < m_UComVec.size(); ++i)
		{
			if (m_SelectUnit == m_UComVec[i])
			{
				UBoxList.SetCurSel(i);
				break;
			}
		}
	}
}