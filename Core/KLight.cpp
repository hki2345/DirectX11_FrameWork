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

	m_LD.m_Dir.w = .0f;
	m_LD.m_Pos.w = .0f;
	m_LD.CamPos.w = .0f;
}

void KLight::Render()
{
	Core_Class::MainDevice().Set_DeviceCB<LightData>(L"LIGHT", m_LD, SHADER_TYPE::ST_PS);

	if (nullptr == m_LMat || nullptr ==m_LMesh)
	{
		KASSERT(true);
	}

	m_LMat->Update();
	m_LMat->Update_Tex();
	m_LMesh->Update();
	m_LMesh->Render();
	m_LMat->Reset();
}

void KLight::type(const LightType& _Type)
{
	switch (_Type)
	{
	case KLight::Direction:
		m_LMesh = ResourceManager<Mesh>::Find(L"RECT3D_MESH");
		m_LMat = ResourceManager<Material>::Find(L"");
		break;

	case KLight::Point:
		m_LMesh = ResourceManager<Mesh>::Find(L"Sphere");
		break;

	default:
		break;
	}
}