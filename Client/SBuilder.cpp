#include "SBuilder.h"

#include <Camera.h>
#include <Light.h>
#include <Renderer_Mesh.h>

#include <Effect_Bloom.h>



SBuilder::SBuilder()
{
}


SBuilder::~SBuilder()
{
}

void SBuilder::Build_State()
{

	KPtr<Camera> m_pCam = state()->Create_One(L"Camera")->Add_Component<Camera>();
	m_pCam->Far(10000.0f);
	m_pCam->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));
	m_pCam->Insert_LayerData(0);
	m_pCam->ProjectionMode(Camera::PM_PERS);
	m_pCam->Create_EffectPost<Effect_Bloom>(0);

	KPtr<Renderer_Mesh> BSphere = state()->Create_One(L"BackGround")->Add_Component<Renderer_Mesh>();
	BSphere->one()->Trans()->scale_local(KVector4(500.0f, 500.0f, 500.0f));
	BSphere->one()->Trans()->pos_local(KVector4(.0f, 0.0f, 0.0f));
	BSphere->one()->Trans()->rotate_local(KVector4(.0f, 130.0f, 0.0f));
	BSphere->Set_Material(L"SKY3DMATFORWARD");
	BSphere->Set_RSState(L"SNONE");
	BSphere->ROpt.Defferd_orForward = 0;
	BSphere->ROpt.LightOpacity = 1.0f;
	BSphere->Set_Mesh(L"SPHERE");
	BSphere->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"mobius_skybox_nebula_diff.png");



	KPtr<Light> pLight4 = state()->Create_One()->Add_Component<Light>();;
	pLight4->one()->Trans()->rotate_world(KVector4(180.0F, 45.0F, 45.0f));
	pLight4->PushLightLayer(0);
}