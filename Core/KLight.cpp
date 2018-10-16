#include "KLight.h"
#include "State.h"
#include "ResourceManager.h"
#include "Core_Class.h"
#include "Mesh.h"
#include "Material.h"


KLight::KLight()
{
	m_LD.LC.Color = KColor::White;
	m_LD.LC.Diff = KVector(1.0f, 1.0f, 1.0f, 1.0f);
	m_LD.LC.Spec = KVector(.2f, .2f, .2f, 1.0f);
	m_LD.LC.Ambi = KVector(1.0f, 1.0f, 1.0f, 1.0f);
	type(LightType::Direction);
}


KLight::~KLight()
{
}

bool KLight::Init()
{
	state()->this_RenderManager.Insert_Light(this);
	return true;
}


void KLight::FinalUpdate()
{
	m_LD.m_Dir = one()->World_Forward(); 
	m_LD.m_Pos = one()->World_Pos();
	m_LD.CamPos = state()->camera()->one()->World_Pos();
	m_LD.Type = m_eType;

	m_LD.m_Dir.w = .0f;
	m_LD.m_Pos.w = .0f;
	m_LD.CamPos.w = .0f;
}

void KLight::Render(KPtr<Camera> _Cam)
{
	/**************** Direction *****************/
	Core_Class::MainDevice().Set_DeviceCB<LightData>(L"DEFLIGHT_DATA", m_LD, SHADER_TYPE::ST_PS);

	if (nullptr == m_LMat || nullptr ==m_LMesh)
	{
		KASSERT(true);
	}



	/****************** Point ******************/
	if (m_eType == LightType::Point)
	{
		Core_Class::MainDevice().Set_DepthSencilMode(L"PASS_ST", 1);


		KMatrix Scale;
		Scale.Scale(KVector(m_Trans->World_Scale().x));

		KMatrix World;
		World.Identity();
		World = Scale * m_Trans->Rotate_Matrix_Const() * m_Trans->Position_Matrix_Const();

		m_MD.m_W = World.TransPose_Referance();
		m_MD.m_V = _Cam->View();
		m_MD.m_P = _Cam->Proj();
		m_MD.m_WV = (World* _Cam ->View()).TransPose_Referance();
		m_MD.m_WVP = (World* _Cam->View_Proj()).TransPose_Referance();

		Core_Class::MainDevice().Set_DeviceCB<DATA_3D>(L"DATA3D", m_MD, SHADER_TYPE::ST_VS);
		Core_Class::MainDevice().Set_DeviceCB<DATA_3D>(L"DATA3D", m_MD, SHADER_TYPE::ST_PS);

		Core_Class::MainDevice().Set_RasterMode(L"SFRONT");
		Core_Class::MainDevice().Set_DepthSencilMode(L"BACK_ST", 1);

		m_VolumeMat->Update();
		m_VolumeMesh->Update();
		m_VolumeMesh->Render();
	}
	else if (m_eType == LightType::Spot)
	{

	}
	else
	{
		Core_Class::MainDevice().Set_DepthSencilMode(L"PASS_ST", 1);
	}

	m_LMat->Update();
	m_LMat->Update_Tex();
	m_LMesh->Update();
	m_LMesh->Render();
	m_LMat->Reset();
}

void KLight::type(const LightType& _Type)
{
	m_eType = _Type;

	switch (_Type)
	{
	case KLight::Direction:
		m_LMesh = ResourceManager<Mesh>::Find(L"RECT3D");
		m_LMat = ResourceManager<Material>::Find(L"LIGHTDEF_MAT");
		break;

	case KLight::Point:
		m_VolumeMesh = ResourceManager<Mesh>::Find(L"SPHERE");
		m_LMesh = ResourceManager<Mesh>::Find(L"RECT3D");
		break;

	default:
		break;
	}

	m_VolumeMat = ResourceManager<Material>::Find(L"VOLUME_MAT");
	m_LMat = ResourceManager<Material>::Find(L"LIGHTDEF_MAT");
}