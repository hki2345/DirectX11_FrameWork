// HToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "HToolDlg.h"
#include "afxdialogex.h"
#include <Core_Class.h>
#include <HScene.h>
#include <HActor.h>
#include <H3DRectRenderer.h>
#include <HFreeCamera.h>
#include <H3DGRID.h>
#include <H3DMeshRenderer.h>
#include <HResMgr.h>
#include <HImage.h>
#include <HLight.h>


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

	HResMgr<HTexture>::Load(L"Texture", L"BUMPTEST.png");
	HResMgr<HTexture>::Load(L"Texture", L"BUMPTEST_N.png");

	HResMgr<HTexture>::Load(L"Texture", L"TILE_01.png");
	HResMgr<HTexture>::Load(L"Texture", L"TILE_01_N.png");


	HFBX FBXLoader;
	FBXLoader.Load((GamePath::FindPath_ToString(L"Mesh") + L"Monster.FBX").c_str());

	KPtr<HScene> TabScene = Core_Class::MainSceneMgr().FindScene(SceneName.GetString());

	if (nullptr == TabScene) 
	{
		KASSERT(true);
	}

	TabScene->Camera()->AddCom<HFreeCamera>();
	TabScene->Camera()->Far(10000.0f);
	TabScene->Camera()->Actor()->Trans()->LPos(HVEC(0.0f, 10.0f, -20.0f));


	KPtr<HActor> Light3 = TabScene->CreateActor();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->LScale(HVEC(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<HLight> pLight3 = Light3->AddCom<HLight>();
	pLight3->SetType(HLight::LIGHTTYPE::POINT);
	pLight3->Trans()->WPos(HVEC(25.0F, 0.0F, 0.0f));
	pLight3->Trans()->WScale(HVEC(20.0f, 20.0f, 20.0f));
	pLight3->PushLightLayer(0, 1, 2, 3, 4, 5);


	KPtr<HActor> Light = TabScene->CreateActor();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->LScale(HVEC(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<HLight> pLight = Light->AddCom<HLight>();
	pLight->SetType(HLight::LIGHTTYPE::POINT);
	pLight->Trans()->WScale(HVEC(30.0f, 30.0f, 30.0f));
	pLight->PushLightLayer(0, 1, 2, 3, 4, 5);


	KPtr<HActor> Light2 = TabScene->CreateActor();
	// 스케일은 dir이 아닌 빛의 크기를 나타낸다.
	// Light->Trans()->LScale(HVEC(1000.0f, 1000.0f, 1000.0f));
	// Foward로 비춘다고 생각한다.
	KPtr<HLight> pLight2 = Light2->AddCom<HLight>();
	pLight2->Trans()->WRot(HVEC(45.0F, 0.0F, 0.0f));
	pLight2->Trans()->WScale(HVEC(30.0f, 30.0f, 30.0f));
	pLight2->PushLightLayer(0, 1, 2, 3, 4, 5);



	KPtr<HActor> GRIDACTOR = TabScene->CreateActor();
	GRIDACTOR->Trans()->WRot(HVEC(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->WScale(HVEC(10000.0f, 10000.0f, 10000.0f));
	GRIDACTOR->AddCom<H3DGRID>();

	KPtr<HActor> SPHERELEFT = TabScene->CreateActor();
	SPHERELEFT->Trans()->LScale(HVEC(10.0f, 10.0f, 10.0f));
	SPHERELEFT->Trans()->LPos(HVEC(-15.0f, 0.0f, 0.0f));
	KPtr<H3DMeshRenderer> PTRMESH1 = SPHERELEFT->AddCom<H3DMeshRenderer>();
	PTRMESH1->ROpt.IsLight = 1;
	PTRMESH1->SetMat(L"MESH3DMAT");
	PTRMESH1->SetMesh(L"SPHERE");

	PTRMESH1->Mat()->AddTexData(TEXTYPE::TT_COLOR, 0, L"TILE_01.png");
	PTRMESH1->Mat()->AddTexData(TEXTYPE::TT_BUMP, 1, L"TILE_01_N.png");

	// PTRMESH1->Mat()->AddTexData(TEXTYPE::TT_COLOR, 0, L"BUMPTEST.png");
	// PTRMESH1->Mat()->AddTexData(TEXTYPE::TT_BUMP, 1, L"BUMPTEST_N.png");

	KPtr<HActor> SPHERERIGHT = TabScene->CreateActor();
	SPHERERIGHT->Trans()->LScale(HVEC(10.0f, 10.0f, 10.0f));
	SPHERERIGHT->Trans()->LPos(HVEC(15.0f, 0.0f, 0.0f));
	KPtr<H3DMeshRenderer> PTRMESH2 = SPHERERIGHT->AddCom<H3DMeshRenderer>();
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
