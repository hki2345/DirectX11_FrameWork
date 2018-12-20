// Dlg_Terrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_Terrain.h"
#include "afxdialogex.h"


#include <Core_Class.h>
#include <SC2_Camera.h>
#include <Light.h>
#include <Texture_Multi.h>


#include <Renderer_Mesh.h>
#include <Renderer_Grid.h>
#include <Renderer_Terrain.h>

#include <Renderer_BonAni.h>
#include <ResourceManager.h>

#include <KRay3D.h>

// Dlg_Terrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Terrain, TabDlg)

Dlg_Terrain::Dlg_Terrain(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_TERRAINDLG, pParent)
{

}

Dlg_Terrain::~Dlg_Terrain()
{
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


	TabScene->Camera()->Add_Component<SC2_Camera>();
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
	TERRAIN->Trans()->scale_local(KVector4(1.0f, 2.0f, 1.0f));
	TERRAIN->Trans()->pos_world(KVector4(5.0f, .0f, .0f, .0f));
	m_pTer = TERRAIN->Add_Component<Renderer_Terrain>();

	 // 순서를 지켜야 된다????? ㅇㅇ
	m_pTer->Create_Terrain(64, 64, L"Cover.jpg", 1.0f);
	m_pTer->base_texture(L"FB");
	m_pTer->Insert_CoverTex(L"FC", L"Cover.jpg");
	m_pTer->Set_RSState(L"SFRONT");
	m_pTer->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 7, L"cursor-target-allied.dds");
	m_pTer->brush_size(10.0f);



	TabScene->This_Col3DManager.Link(101, 100);
	TabScene->This_Col3DManager.Link(100, 101);

	KPtr<KRay3D> RayCol = TabScene->Camera()->Add_Component<KRay3D>(101);

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

	DDX_Control(pDX, IDC_TERUNITLIST, UList);
}


void Dlg_Terrain::Init_Dlg()
{
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
	ON_BN_CLICKED(IDC_TEREDITBTN, &Dlg_Terrain::OnBnClickedTereditbtn)
	ON_BN_CLICKED(IDC_STATERESLIST, &Dlg_Terrain::OnBnClickedStatereslist)
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
}


void Dlg_Terrain::OnBnClickedStateload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Dlg_Terrain::OnBnClickedStatesave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	switch (TempId)
	{
	case 0:
	{
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

void Dlg_Terrain::OnBnClickedTereditbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Dlg_Terrain::OnBnClickedStatereslist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}
