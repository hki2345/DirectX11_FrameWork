// Dlg_Editor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "Dlg_Editor.h"
#include "afxdialogex.h"
#include <Core_Class.h>
#include <State.h>
#include <TheOne.h>
#include <Renderer_Rect.h>
#include <Renderer_Grid.h>
#include <Renderer_Mesh.h>
#include <Renderer_Terrain.h>
#include <Texture_Multi.h>

#include <DebugManager.h>
#include <TimeManager.h>
#include <KThread.h>

#include <ResourceManager.h>
#include <KImage.h>
#include <Light.h>

#include "Edit_Class.h"

#include <KSphere_Col.h>
#include <KRay3D.h>

#include <SC2_Camera.h>

// Dlg_Editor ��ȭ �����Դϴ�.

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
END_MESSAGE_MAP()


// Dlg_Editor �޽��� ó�����Դϴ�.




BOOL Dlg_Editor::OnInitDialog()
{
	TabDlg::OnInitDialog();

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(SceneName.GetString());

	if (nullptr == TabScene) 
	{
		KASSERT(true);
	}

	// TabScene->Camera()->Change_Mode();
	TabScene->Camera()->Add_Component<SC2_Camera>();
	TabScene->Camera()->Far(10000.0f);
	TabScene->Camera()->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));
	

	KPtr<TheOne> Light2 = TabScene->Create_One();
	// �������� dir�� �ƴ� ���� ũ�⸦ ��Ÿ����.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward�� ����ٰ� �����Ѵ�.
	KPtr<Light> pLight2 = Light2->Add_Component<Light>();
	pLight2->Trans()->rotate_world(KVector4(45.0F, 0.0F, 0.0f));
	pLight2->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight2->PushLightLayer(0);


	KPtr<TheOne> Light3 = TabScene->Create_One();
	// �������� dir�� �ƴ� ���� ũ�⸦ ��Ÿ����.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward�� ����ٰ� �����Ѵ�.
	KPtr<Light> pLight3 = Light3->Add_Component<Light>();
	pLight3->Trans()->rotate_world(KVector4(-45.0F, 0.0F, 0.0f));
	pLight3->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight3->PushLightLayer(1);


	KPtr<TheOne> Light4 = TabScene->Create_One();
	// �������� dir�� �ƴ� ���� ũ�⸦ ��Ÿ����.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward�� ����ٰ� �����Ѵ�.
	KPtr<Light> pLight4 = Light4->Add_Component<Light>();
	pLight4->Trans()->rotate_world(KVector4(.0F, 45.0F, 0.0f));
	pLight4->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight4->PushLightLayer(0);



	CCreateContext         m_context;

	m_context.m_pNewViewClass = RUNTIME_CLASS(View_Component);
	View_Component* pView = (View_Component*)((CFrameWnd*)this)->CreateView(&m_context);

	RECT ViewSize = { 10, 10, 700, 530 };
	pView->ShowWindow(SW_SHOW);
	pView->MoveWindow(&ViewSize);

	Edit_Class::m_gVIewCom->Release_AllDlg();
	Edit_Class::m_gVIewCom->Set_One(Light2);





	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	GRIDACTOR->Add_Component<Renderer_Grid>();


	
	KPtr<TheOne> SPHERERIGHT = TabScene->Create_One();
	SPHERERIGHT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERERIGHT->Trans()->pos_local(KVector4(15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH2 = SPHERERIGHT->Add_Component<Renderer_Mesh>();
	PTRMESH2->Set_Material(L"DEFFERD3DMAT");
	PTRMESH2->Set_Mesh(L"SPHERE");


	KPtr<TheOne> CUBEMIDDLE = TabScene->Create_One();
	CUBEMIDDLE->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	CUBEMIDDLE->Trans()->pos_local(KVector4(.0f, 50.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH3 = CUBEMIDDLE->Add_Component<Renderer_Mesh>();
	PTRMESH3->Set_Material(L"DEFFERD3DMAT");
	PTRMESH3->Set_Mesh(L"CUBE");
	
	

	// ��ó���� ����� ����������. ������ �ى�
	/*
	KPtr<TheOne> TestAni2 = TabScene->Create_One(L"Test");
	TestAni2->Trans()->pos_local(KVector(.0f));
	TestAni2->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	KPtr<Renderer_BonAni> TestRender2 = TestAni2->Add_Component<Renderer_BonAni>();

	ResourceManager<MeshContainer>::Load((PathManager::Find_ForderPathStr(L"Mesh") + L"Zerg\\ColonistTransPortTread.FBX").c_str());
	TestRender2->Set_Fbx(L"ColonistTransPortTread.FBX");
	TestRender2->Create_AniChanger(L"TestAni", 0, 70000);
	TestRender2->Set_AniChanger(L"TestAni");*/


	// TestRender->Load_FbxTest((PathManager::Find_ForderPathStr(L"Mesh") + L"Monster3.FBX").c_str());
	//TestRender->Load_FbxTest((PathManager::Find_ForderPathStr(L"Mesh") + L"Warehouse01.FBX").c_str());
	//TestRender->Set_Static();


	// ��Ƽ �ؽ���
	KPtr<Texture_Multi> MTex = ResourceManager<Texture_Multi>::Create(L"FB");
	MTex->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"Stone.jpg", L"StoneBump.jpg");


	KPtr<Texture_Multi> MTex2 = ResourceManager<Texture_Multi>::Create(L"FC");
	MTex2->Create_MultiTex(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"Lava.jpg", L"LavaBump.jpg");


	KPtr<TheOne> TERRAIN = TabScene->Create_One();
	TERRAIN->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	TERRAIN->Trans()->pos_local(KVector4(.0f, 0.0f, 0.0f));
	KPtr<Renderer_Terrain> TerMESH1 = TERRAIN->Add_Component<Renderer_Terrain>();

	// ������ ���Ѿ� �ȴ�????? ����
	TerMESH1->Create_Terrain(64, 64, L"Cover.jpg", 1.0f);
	TerMESH1->base_texture(L"FB");
	TerMESH1->Insert_CoverTex(L"FC", L"Cover.jpg");
	TerMESH1->Set_RSState(L"SFRONT");


	KPtr<TheOne> SPHERELEFT = TabScene->Create_One();
	SPHERELEFT->Trans()->scale_local(KVector4(10.0f, 10.0f, 10.0f));
	SPHERELEFT->Trans()->pos_local(KVector4(-15.0f, 0.0f, 0.0f));
	KPtr<Renderer_Mesh> PTRMESH1 = SPHERELEFT->Add_Component<Renderer_Mesh>();
	PTRMESH1->ROpt.IsLight = 1;
	PTRMESH1->Set_Material(L"DEFFERD3DMAT");
	PTRMESH1->Set_Mesh(L"SPHERE");

	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"MoonDiff.jpg");
	PTRMESH1->material()->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, L"MoonBump.jpg");


	TabScene->This_Col3DManager.Link(101, 100);
	TabScene->This_Col3DManager.Link(100, 101);

	// KPtr<KRay3D> RayCol = TabScene->Camera()->Add_Component<KRay3D>(101);
	// RayCol->EnterFunc(this, &Dlg_Editor::Collision_Test);
	// 
	// KPtr<KSphere_Col> RightCol = SPHERERIGHT->Add_Component<KSphere_Col>(100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void Dlg_Editor::Collision_Test(KCollision*, KCollision*)
{

}
