// Dlg_Editor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "Dlg_Editor.h"
#include "afxdialogex.h"
#include <Core_Class.h>
#include <State.h>
#include <TheOne.h>
#include <Renderer_Rect.h>
#include <Camera_Free.h>
#include <Renderer_Grid.h>
#include <Renderer_Mesh.h>
#include <Renderer_BonAni.h>

#include <ResourceManager.h>
#include <KImage.h>
#include <Light.h>

#include "Edit_Class.h"


// Dlg_Editor 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Editor, TabDlg)

Dlg_Editor::Dlg_Editor(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_TOOLDLG, pParent)
{

}

Dlg_Editor::~Dlg_Editor()
{
}

void Dlg_Editor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_Editor, TabDlg)
	ON_BN_CLICKED(IDOK, &Dlg_Editor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Dlg_Editor::OnBnClickedCancel)
END_MESSAGE_MAP()


// Dlg_Editor 메시지 처리기입니다.


void Dlg_Editor::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Dlg_Editor::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL Dlg_Editor::OnInitDialog()
{
	TabDlg::OnInitDialog();

	

	// FBXLoader FLoad;
	// FLoad.Load_FBX((PathManager::Find_ForderPathStr(L"Mesh") + L"Warehouse01.FBX").c_str());

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(SceneName.GetString());

	if (nullptr == TabScene) 
	{
		KASSERT(true);
	}

	TabScene->Camera()->Add_Component<Camera_Free>();
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



	CCreateContext         m_context;

	m_context.m_pNewViewClass = RUNTIME_CLASS(View_Component);
	View_Component* pView = (View_Component*)((CFrameWnd*)this)->CreateView(&m_context);

	RECT ViewSize = { 200, 10, 700, 530 };
	pView->ShowWindow(SW_SHOW);
	pView->MoveWindow(&ViewSize);

	Edit_Class::m_gVIewCom->Release_AllDlg();
	Edit_Class::m_gVIewCom->Set_One(Light1);





	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	GRIDACTOR->Add_Component<Renderer_Grid>();


	
	KPtr<TheOne> SPHERERIGHT = TabScene->Create_One();
	SPHERERIGHT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERERIGHT->Trans()->pos_local(KVector4(15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH2 = SPHERERIGHT->Add_Component<Renderer_Mesh>();
	//PTRMESH2->SetMat(L"PIXLIGHT3DMAT");
	PTRMESH2->Set_Mesh(L"SPHERE");


	KPtr<TheOne> CUBEMIDDLE = TabScene->Create_One();
	CUBEMIDDLE->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	CUBEMIDDLE->Trans()->pos_local(KVector4(.0f, 50.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH3 = CUBEMIDDLE->Add_Component<Renderer_Mesh>();
	PTRMESH3->Set_Mesh(L"CUBE");


	// AniTest
	KPtr<TheOne> TestAni = TabScene->Create_One(L"Test");
	TestAni->Trans()->pos_local(KVector(.0f));
	TestAni->Trans()->scale_local(KVector(.3f, .3f, .3f));
	KPtr<Renderer_BonAni> TestRender = TestAni->Add_Component<Renderer_BonAni>();

	TestRender->Load_FbxTest((PathManager::Find_ForderPathStr(L"Mesh") + L"Monster3.FBX").c_str());
	// TestRender->Load_FbxTest((PathManager::Find_ForderPathStr(L"Mesh") + L"Warehouse01.FBX").c_str());
	
	KPtr<TheOne> SPHERELEFT = TabScene->Create_One();
	SPHERELEFT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERELEFT->Trans()->pos_local(KVector4(-15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH1 = SPHERELEFT->Add_Component<Renderer_Mesh>();
	PTRMESH1->ROpt.IsLight = 1;
	PTRMESH1->Set_Material(L"MESH3DMAT");
	PTRMESH1->Set_Mesh(L"SPHERE");

	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"MoonDiff.jpg");
	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, L"MoonBump.jpg");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
