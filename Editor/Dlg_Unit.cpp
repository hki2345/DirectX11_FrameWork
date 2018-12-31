// Dlg_Unit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_Unit.h"
#include "afxdialogex.h"

#include <Core_Class.h>
#include <Renderer_BonAni.h>
#include <Renderer_Grid.h>
#include <Renderer_Mesh.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <ResourceManager.h>

#include <Light.h>

#include "Edit_Class.h"

#include <Force_Unit.h>
#include <Controll_User.h>
#include <SC2_Camera.h>

// Dlg_Unit 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Unit, TabDlg)

Dlg_Unit::Dlg_Unit(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_UNITDLG, pParent)
	, m_CurOne(nullptr)
{

}

Dlg_Unit::~Dlg_Unit()
{
}




void Dlg_Unit::Init_Dlg()
{
	//ResourceManager<MeshContainer>::Clear();
	//ResourceManager<Changer_Animation>::Clear();


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 만약 자체 메쉬에 애니메이션을 바로 적용시키려면
	// 애니메이션이 먼저 불러온 후 배급받는 식이다.
	
	if (0 == ResourceManager<Changer_Animation>::All_Count())
	{
		ResourceManager<Changer_Animation>::All_Load();
		ResourceManager<MeshContainer>::All_Load();
	}

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(StateName.GetString());
	m_CurOne = TabScene->Create_One(L"Unit_Model");
	m_CurOne->Trans()->pos_local(KVector(.0f));
	m_CurOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));

	m_CurUnit = m_CurOne->Add_Component<Force_Unit>(L"Marine");
	m_CurOne->Add_Component<Controll_User>(m_pTer, m_CurUnit, TabScene->Camera()->Get_Component<SC2_Camera>());


	CString TmpStr;

	InfoValue[0].SetWindowTextW(L".0");
	InfoValue[1].SetWindowTextW(L".0");
	InfoValue[2].SetWindowTextW(L"UNITNAME");
	InfoValue[3].SetWindowTextW(L"1.0");
	InfoValue[4].SetWindowTextW(L"1.0");
	InfoValue[5].SetWindowTextW(L"1.0");

	WeaponType[0].SetCheck(true);
	WeaponType[1].SetCheck(false);
	WeaponType[2].SetCheck(false);



	Update_RscTree();
}


void Dlg_Unit::Update_Dlg()
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
}


BOOL Dlg_Unit::OnInitDialog()
{
	TabDlg::OnInitDialog();

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
	SKYMESH1->Set_Material(L"SKY3DMAT");
	SKYMESH1->Set_RSState(L"SNONE");
	SKYMESH1->ROpt.Defferd_orForward = 1;
	SKYMESH1->ROpt.LightOpacity = 1.0f;
	SKYMESH1->Set_Mesh(L"SPHERE");
	SKYMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"Space.jpg");

	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	KPtr<Renderer_Grid> GRIDRENDER = GRIDACTOR->Add_Component<Renderer_Grid>();
	GRIDRENDER->ROpt.Defferd_orForward = 1;

	
	m_pCam = TabScene->Camera()->Add_Component<SC2_Camera>();
	TabScene->Camera()->Far(10000.0f);
	TabScene->Camera()->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));


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
	m_pTer->Edit_Off();


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

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Dlg_Unit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UNITKM3LIST, m_KM3List);
	DDX_Control(pDX, IDC_UNITSELECTLIST, m_SelectList);


	int StartId = IDC_ULEDIT;
	
	// 스피드 * 2, 이름, 스케일
	for (size_t X = 0;  X < 6; ++X)
	{
		DDX_Control(pDX, StartId, InfoValue[X]);
		++StartId;
	}


	StartId = IDC_WNONEBTN;

	// 스피드 * 2, 이름, 스케일
	for (size_t X = 0; X < 3; ++X)
	{
		DDX_Control(pDX, StartId, WeaponType[X]);
		++StartId;
	}
}


void Dlg_Unit::Update_RscTree()
{

	UpdateData(TRUE);

	m_KM3List.ResetContent();
	std::vector<KPtr<MeshContainer>> TVec = ResourceManager<MeshContainer>::All_SingleVec();

	for (size_t i = 0; i < TVec.size(); i++)
	{
		std::wstring Temp = TVec[i]->FileExt();
		if (L".KM3" == Temp)
		{
			m_KM3List.AddString(TVec[i]->FileNameExt());
		}
	}

	UpdateData(FALSE);
}

void Dlg_Unit::Update_AddModel()
{
	int Cnt = m_SelectList.GetCount();
	if (0 == Cnt)
	{
		m_CurOne->Active_Off();
		return;
	}

	for (int i = 0; i < Cnt; i++)
	{
		wchar_t Name[NAMENUM];
		m_SelectList.GetText(i, Name);


		bool Check = false;
		m_SRI = m_RenderList.begin();
		m_ERI = m_RenderList.end();

		for (; m_SRI != m_ERI; ++m_SRI)
		{
			// 있는 거다.
			if (true == PathManager::Is_StrVSStr(Name, (*m_SRI)->mesh_container()->FileNameExt()))
			{
				Check = true;
				break;
			}
		}

		if (true == Check)
		{
			continue;
		}

		KPtr<Renderer_BonAni> Tmp = m_CurOne->Add_Component<Renderer_BonAni>();
		Tmp->Set_Fbx(Name);

		if (nullptr == Tmp->changer_animation())
		{
			Tmp->Create_Animation();
		}

		m_CurUnit->Insert_Renderer(Tmp);
		m_CurOne->Get_Component<Controll_User>()->Add_Render(Tmp);
		m_RenderList.push_back(Tmp);
	}

	m_CurOne->Active_On();

}

void Dlg_Unit::Update_DelModel()
{
	int Cnt = m_SelectList.GetCount();
	if (0 == Cnt)
	{
		m_CurOne->Active_Off();
		return;
	}

	for (int i = 0; i < Cnt; i++)
	{
		bool Check = false;
		m_SRI = m_RenderList.begin();
		m_ERI = m_RenderList.end();

		for (; m_SRI != m_ERI; ++m_SRI)
		{
			// 있는 거다.
			if (true == PathManager::Is_StrVSStr(m_CurStr.c_str(), (*m_SRI)->mesh_container()->FileNameExt()))
			{
				(*m_SRI)->Set_Death();
				m_CurUnit->Delete_Renderer((*m_SRI));
				m_CurOne->Get_Component<Controll_User>()->Del_Render((*m_SRI));
				m_RenderList.erase(m_SRI);
				return;
			}
		}
	}

	m_CurOne->Active_On();

}

void Dlg_Unit::Update_SelectList()
{
	m_SelectList.ResetContent();
	m_RenderList = m_CurUnit->list_renderer();


	m_SRI = m_RenderList.begin();
	m_ERI = m_RenderList.end();


	for (; m_SRI != m_ERI; ++m_SRI)
	{
		m_SelectList.AddString((*m_SRI)->mesh_container()->FileNameExt());
	}
}


// Dlg_Unit 메시지 처리기입니다.
BEGIN_MESSAGE_MAP(Dlg_Unit, TabDlg)
	ON_BN_CLICKED(IDC_RENINSERTBTN, &Dlg_Unit::OnBnClickedReninsertbtn)
	ON_BN_CLICKED(IDC_RENDELETEBTN, &Dlg_Unit::OnBnClickedRendeletebtn)
	ON_BN_CLICKED(IDC_UNITLOADBTN, &Dlg_Unit::OnBnClickedUnitloadbtn)
	ON_BN_CLICKED(IDC_UNITSAVEBTN, &Dlg_Unit::OnBnClickedUnitsavebtn)
	ON_LBN_SELCHANGE(IDC_UNITKM3LIST, &Dlg_Unit::OnLbnSelchangeUnitkm3list)
	ON_BN_CLICKED(IDC_RENRESTARTBTN, &Dlg_Unit::OnBnClickedRenrestartbtn)


	ON_CONTROL_RANGE(EN_CHANGE, IDC_ULEDIT, IDC_KUNITZINFO, &Dlg_Unit::UnitInfoSelchange)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_WNONEBTN, IDC_WSHOTBTN, &Dlg_Unit::UnitWeaponSelchange)
END_MESSAGE_MAP()


void Dlg_Unit::OnBnClickedReninsertbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (LB_ERR == m_SelectList.FindString(0, m_CurStr.c_str()) && m_CurStr != L"")
	{
		m_SelectList.AddString(m_CurStr.c_str());
	}

	Update_AddModel();
}


void Dlg_Unit::OnBnClickedRendeletebtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Tint = m_SelectList.GetCurSel();

	CString tmpCstr;
	m_CurStr;
	m_SelectList.GetText(Tint, tmpCstr);

	m_CurStr = tmpCstr.GetString();
	m_SelectList.DeleteString(Tint);

	Update_DelModel();
}




void Dlg_Unit::OnBnClickedUnitloadbtn()
{
	m_RenderList.clear();


	CString Tmp;
	InfoValue[2].GetWindowTextW(Tmp);


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_CurUnit->Load(Tmp.GetBuffer());


	// 이름
	InfoValue[2].SetWindowTextW(Tmp);

	std::wstring TmpStr;
	TmpStr = std::to_wstring(m_CurUnit->linear_speed());
	InfoValue[0].SetWindowTextW(TmpStr.c_str());

	TmpStr = std::to_wstring(m_CurUnit->rotate_speed());
	InfoValue[1].SetWindowTextW(TmpStr.c_str());

	TmpStr = std::to_wstring(m_CurUnit->unit_scale().x);
	InfoValue[3].SetWindowTextW(TmpStr.c_str());

	TmpStr = std::to_wstring(m_CurUnit->unit_scale().y);
	InfoValue[4].SetWindowTextW(TmpStr.c_str());

	TmpStr = std::to_wstring(m_CurUnit->unit_scale().z);
	InfoValue[5].SetWindowTextW(TmpStr.c_str());




	WeaponType[0].SetCheck(false);
	WeaponType[1].SetCheck(false);
	WeaponType[2].SetCheck(false);

	switch (m_CurUnit->weapon_type())
	{
	case WEAPON_TYPE::NONE:
		WeaponType[0].SetCheck(true);
		break;

	case WEAPON_TYPE::INSTANCE:
		WeaponType[1].SetCheck(true);
		break;

	case WEAPON_TYPE::SHOT:
		WeaponType[2].SetCheck(true);
		break;
	default:
		break;
	}

	Update_SelectList();
}


void Dlg_Unit::OnBnClickedUnitsavebtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_CurUnit->Save();
}


void Dlg_Unit::OnLbnSelchangeUnitkm3list()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Tint = m_KM3List.GetCurSel();
	
	CString tmpCstr;
	m_CurStr;
	m_KM3List.GetText(Tint, tmpCstr);

	m_CurStr = tmpCstr.GetString();
}


void Dlg_Unit::OnBnClickedRenrestartbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_SRI = m_RenderList.begin();
	m_ERI = m_RenderList.end();


	for (; m_SRI != m_ERI; ++m_SRI)
	{
		(*m_SRI)->Reset_Frame();
	}
}


void Dlg_Unit::UnitInfoSelchange(UINT _Id)
{
	UINT TempId = _Id - IDC_ULEDIT;
	CString TempStr;
	Force_Unit::Unit_Info NI;


	InfoValue[TempId].GetWindowTextW(TempStr);

	float Tmpf = (float)_wtof(TempStr.GetBuffer());

	// 선형 속도
	if (0 == TempId)
	{
		m_CurUnit->linear_speed(Tmpf);
	}
	// 각속도
	else if (1 == TempId)
	{
		m_CurUnit->rotate_speed(Tmpf);
	}
	// 이름 - 스트링
	else if (2 == TempId)
	{
		m_CurUnit->name(TempStr.GetBuffer());
	}
	// XScale
	else if (3 == TempId)
	{
		m_CurUnit->unit_scale(
			KVector(Tmpf, m_CurUnit->unit_scale().y, m_CurUnit->unit_scale().z, .0f));
	}
	// YScale
	else if (4 == TempId)
	{
		m_CurUnit->unit_scale(
			KVector(m_CurUnit->unit_scale().x, Tmpf, m_CurUnit->unit_scale().z, .0f));
	}
	// ZScale
	else if (5 == TempId)
	{
		m_CurUnit->unit_scale(
			KVector(m_CurUnit->unit_scale().x, m_CurUnit->unit_scale().y, Tmpf, .0f));
	}

}



void Dlg_Unit::UnitWeaponSelchange(UINT _Id)
{
	UINT TempId = _Id - IDC_WNONEBTN;

	switch (TempId)
	{
	case 0:
		m_CurUnit->weapon_type(WEAPON_TYPE::NONE);
		break;

	case 1:
		m_CurUnit->weapon_type(WEAPON_TYPE::INSTANCE);
		break;

	case 2:
		m_CurUnit->weapon_type(WEAPON_TYPE::SHOT);
		break;

	default:
		break;
	}
}