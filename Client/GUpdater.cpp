#include "GUpdater.h"

#include <CollisionManager.h>
#include <KRay3D.h>
#include <InputManager.h>
#include <Core_Class.h>


#include <Renderer_Mesh.h>
#include <Renderer_Terrain.h>
#include <Effect_Bloom.h>

#include <Camera.h>
#include <Light.h>
#include <KFont.h>


#include <Con_Class.h>
#include <SC2_Force.h>
#include <SC2Manager.h>
#include <SC2_Camera.h>


GUpdater::GUpdater()
{
}


GUpdater::~GUpdater()
{
}

void  GUpdater::Start_State()
{
	Init_Mesh();
	Init_Terrain();
	Init_Unit();
}



void GUpdater::Init_Terrain()
{
	m_pTer = state()->Create_One()->Add_Component<Renderer_Terrain>();
	m_pTer->one()->Trans()->scale_local(KVector4(5.0f, 2.0f, 5.0f));
	m_pTer->one()->Trans()->pos_world(KVector4(5.0f, .0f, .0f, .0f));

	// ������ ���Ѿ� �ȴ�????? ����
	m_pTer->Create_Terrain(64, 64, L"Cover.jpg", 1.0f);
	m_pTer->base_texture(L"FB");
	m_pTer->Insert_CoverTex(L"FC", L"Cover.jpg");
	m_pTer->Set_RSState(L"SNONE");
	m_pTer->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 7, L"cursor-target-allied.dds");
	m_pTer->brush_size(10.0f);

	m_pTer->Load(L"RaynorT");
}


void GUpdater::Init_Mesh()
{
	state()->This_Col3DManager.Link(100, 100);
	state()->This_Col3DManager.Link(0, 100);

	KPtr<Camera> m_TT = state()->Create_One(L"Camera")->Add_Component<Camera>();
	state()->camera(m_TT);
	state()->camera()->Add_Component<KRay3D>(100);
	m_TT->Far(10000.0f);
	m_TT->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));
	m_TT->Insert_LayerData(0);
	m_TT->ProjectionMode(Camera::PM_PERS);
	m_TT->Create_EffectPost<Effect_Bloom>(0);
	m_pCam = m_TT->Add_Component<SC2_Camera>();

	KPtr<Light> pLight4 = state()->Create_One()->Add_Component<Light>();;
	pLight4->one()->Trans()->rotate_world(KVector4(180.0F, 45.0F, 45.0f));
	pLight4->PushLightLayer(0);


	SkySphere = state()->Create_One(L"BackGround")->Add_Component<Renderer_Mesh>();
	SkySphere->one()->Trans()->scale_local(KVector4(10000.0f, 10000.0f, 10000.0f));
	SkySphere->one()->Trans()->pos_local(KVector4(.0f, 0.0f, 0.0f));
	SkySphere->one()->Trans()->rotate_local(KVector4(.0f, 130.0f, 0.0f));
	SkySphere->Set_Material(L"SKY3DMATFORWARD");
	SkySphere->Set_RSState(L"SNONE");
	SkySphere->ROpt.Defferd_orForward = 0;
	SkySphere->ROpt.LightOpacity = 1.0f;
	SkySphere->Set_Mesh(L"SPHERE");
	SkySphere->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"mobius_skybox_nebula_diff.png");
}
void GUpdater::Init_Unit()
{
	Con_Class::s2_manager()->Load(L"BBY", m_pTer);
	Con_Class::s2_manager()->Find_Force(L"BOOMBAYAH")->playable_type(PLAYABLE_TYPE::PBT_USER);
	Con_Class::s2_manager()->Find_Force(L"BAAM")->playable_type(PLAYABLE_TYPE::PBT_NONE);
	Con_Class::s2_manager()->Find_Force(L"LUV")->playable_type(PLAYABLE_TYPE::PBT_NONE);

	Con_Class::s2_manager()->Find_Force(L"REDFLAVER")->playable_type(PLAYABLE_TYPE::PBT_ENEMY);
	Con_Class::s2_manager()->Find_Force(L"POP+STARS")->Set_Active(false);
	Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->Set_Active(false);

	m_pCam->Set_InGame();
}

void  GUpdater::Update_State()
{
	if (true == Con_Class::s2_manager()->m_GameSet)
	{
		Core_Class::MainSceneMgr().Change_State(L"End");
		Con_Class::s2_manager()->Clear_Force();
	}
}

void GUpdater::UIRender()
{
}


void GUpdater::End_State()
{
	Con_Class::s2_manager()->Clear_Force();
}