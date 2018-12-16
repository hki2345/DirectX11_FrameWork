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
	TERRAIN->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	TERRAIN->Trans()->pos_world(KVector4(-5.0f, -5.0f, .0f, .0f));
	KPtr<Renderer_Terrain> TerMESH1 = TERRAIN->Add_Component<Renderer_Terrain>();

	 // 순서를 지켜야 된다????? ㅇㅇ
	 TerMESH1->Create_Terrain(20, 20, L"Cover.jpg", 1.0f);
	 TerMESH1->base_texture(L"FB");
	 TerMESH1->Insert_CoverTex(L"FC", L"Cover.jpg");
	 TerMESH1->Set_RSState(L"SFRONT");



	return TRUE;
}

void Dlg_Terrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


void Dlg_Terrain::Init_Dlg()
{
}


BEGIN_MESSAGE_MAP(Dlg_Terrain, TabDlg)
END_MESSAGE_MAP()


// Dlg_Terrain 메시지 처리기입니다.
