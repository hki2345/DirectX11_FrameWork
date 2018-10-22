// HToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "HToolDlg.h"
#include "afxdialogex.h"
#include <Core_Class.h>
#include <State.h>
#include <TheOne.h>
#include <Renderer_Rect.h>
#include <HFreeCamera.h>
#include <Renderer_Grid.h>
#include <Renderer_Mesh.h>
#include <ResourceManager.h>
#include <KImage.h>
#include <Light.h>


// HToolDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(HToolDlg, TabDlg)

HToolDlg::HToolDlg(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_TOOLDLG, pParent)
{

}

HToolDlg::~HToolDlg()
{
}

void HToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HToolDlg, TabDlg)
	ON_BN_CLICKED(IDOK, &HToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &HToolDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// HToolDlg 메시지 처리기입니다.


void HToolDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void HToolDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL HToolDlg::OnInitDialog()
{
	TabDlg::OnInitDialog();

	// ResourceManager<Texture>::Load(L"Texture", L"BUMPTEST.png");
	// ResourceManager<Texture>::Load(L"Texture", L"BUMPTEST_N.png");
	// 
	// ResourceManager<Texture>::Load(L"Texture", L"TILE_01.png");
	// ResourceManager<Texture>::Load(L"Texture", L"TILE_01_N.png");


	// KFBX FLoad;
	// FLoad.Load((PathManager::Find_ForderPathStr(L"Mesh") + L"BattleTest.FBX").c_str());

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(SceneName.GetString());

	if (nullptr == TabScene) 
	{
		KASSERT(true);
	}

	TabScene->Camera()->Add_Component<HFreeCamera>();
	TabScene->Camera()->Far(10000.0f);
	TabScene->Camera()->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));


	KPtr<TheOne> Light3 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight3 = Light3->Add_Component<Light>();
	pLight3->SetType(Light::LIGHTTYPE::POINT);
	pLight3->Trans()->pos_world(KVector4(25.0F, 0.0F, 0.0f));
	pLight3->Trans()->scale_world(KVector4(20.0f, 20.0f, 20.0f));
	pLight3->PushLightLayer(0, 1, 2, 3, 4, 5);


	KPtr<TheOne> Light1 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight = Light1->Add_Component<Light>();
	pLight->SetType(Light::LIGHTTYPE::POINT);
	pLight->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight->PushLightLayer(0, 1, 2, 3, 4, 5);


	KPtr<TheOne> Light2 = TabScene->Create_One();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<Light> pLight2 = Light2->Add_Component<Light>();
	pLight2->Trans()->rotate_world(KVector4(45.0F, 0.0F, 0.0f));
	pLight2->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight2->PushLightLayer(0, 1, 2, 3, 4, 5);



	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	GRIDACTOR->Add_Component<Renderer_Grid>();

	KPtr<TheOne> SPHERELEFT = TabScene->Create_One();
	SPHERELEFT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERELEFT->Trans()->pos_local(KVector4(-15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH1 = SPHERELEFT->Add_Component<Renderer_Mesh>();
	PTRMESH1->ROpt.IsLight = 1;
	PTRMESH1->SetMat(L"MESH3DMAT");
	PTRMESH1->SetMesh(L"SPHERE");

	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TILE_01.png");
	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, L"TILE_01_N.png");

	// PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"BUMPTEST.png");
	// PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, L"BUMPTEST_N.png");

	KPtr<TheOne> SPHERERIGHT = TabScene->Create_One();
	SPHERERIGHT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERERIGHT->Trans()->pos_local(KVector4(15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH2 = SPHERERIGHT->Add_Component<Renderer_Mesh>();
	//PTRMESH2->SetMat(L"PIXLIGHT3DMAT");
	PTRMESH2->SetMesh(L"SPHERE");

	//if (false == SetMat(L"RECT3DMAT"))
	//{
	//	return false;
	//}
	//if (false == SetMesh(L"RECT"))
	//{
	//	return false;
	//}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
