#include "HLight.h"
#include "HScene.h"
#include "HResMgr.h"
#include "Core_Class.h"
#include "HMesh.h"
#include "HMaterial.h"
#include "DebugManager.h"


HLight::HLight() : m_eType(LIGHTTYPE::DIR)
{
	Data.Lc.Diff = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	Data.Lc.Spec = KVector4(0.2f, 0.2f, 0.2f, 1.0f);
	Data.Lc.Ambi = KVector4(0.1f, 0.1f, 0.1f, 1.0f);
	SetType(LIGHTTYPE::DIR);
}


HLight::~HLight()
{
}


bool HLight::Init() 
{
	Scene()->RenderMgr.PushLight(this);

	return true;
}

void HLight::EndUpdate()
{

}

void HLight::CalLightData(KPtr<HCamera> _Camera) {
	Data.LightDir = Trans()->WForward();
	Data.LightPos = Trans()->WPos();
	Data.Type = m_eType;
	Data.Range = Trans()->WScale().x;

	//wchar_t Arr[256];
	//swprintf_s(Arr, L"LightDir : %f, %f, %f", Data.LightDir.x, Data.LightDir.y, Data.LightDir.z);
	//DebugManager::DrawLog(Arr);
	//swprintf_s(Arr, L"LightPos : %f, %f, %f", Data.LightPos.x, Data.LightPos.y, Data.LightPos.z);
	//DebugManager::DrawLog(Arr);

	Data.LightDir = _Camera->CV().MulVecZero(Data.LightDir);
	Data.LightDirInv = -Data.LightDir;
	Data.LightPos = _Camera->CV().MulVecOne(Data.LightPos);
}

void HLight::LightRender(KPtr<HCamera> _Camera)
{
	// 라이트 데이터
	
	Core_Class::MainDevice().SettingCB<LightData>(L"DEFFERDLIGHTDATA", Data, SHTYPE::ST_PS);

	if (nullptr == m_LightMat || nullptr == m_LightMesh)
	{
		BBY;
	}

	if (m_eType == LIGHTTYPE::POINT)
	{
		Core_Class::MainDevice().SetBsMode(L"VOLUME");
		KMatrix Scale;
		Scale.Scale(KVector4(m_Trans->WScale().x, m_Trans->WScale().x, m_Trans->WScale().x));
		KMatrix World;
		World.Iden();
		World = Scale * m_Trans->CRMat() * m_Trans->CPMat();

		m_MatData.m_W = World.RVTranspose();
		m_MatData.m_V = _Camera->CV().RVTranspose();
		m_MatData.m_P = _Camera->CP().RVTranspose();
		m_MatData.m_WV = (World * _Camera->CV()).RTranspose();
		m_MatData.m_WVP = (World * _Camera->VP()).RTranspose();

		Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATDATA", m_MatData, SHTYPE::ST_VS);
		Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATDATA", m_MatData, SHTYPE::ST_PS);

		// 두번해야 하느냐?
		// 후면보다 앞에 있는 픽셀 검색.
		Core_Class::MainDevice().SetRsMode(L"SFRONT");
		Core_Class::MainDevice().SetDsMode(L"BACK_ST", 1);
		m_VolumeMat->Update();
		m_VolumMesh->Update();
		m_VolumMesh->Render();

		Core_Class::MainDevice().SetRsMode(L"SBACK");
		Core_Class::MainDevice().SetDsMode(L"FRONT_ST", 1);
		m_VolumeMat->Update();
		m_VolumMesh->Update();
		m_VolumMesh->Render();
		Core_Class::MainDevice().SetDsMode(L"PASS_ST", 1);
	}
	else if (m_eType == LIGHTTYPE::SPOT)
	{

	}
	else 
	{
		Core_Class::MainDevice().SetDsMode(L"PASS_ST", 0);
	}


	// 색이 어떻든 더한다.
	Core_Class::MainDevice().SetBsMode(L"LIGHTONE");
	m_LightMat->Update();
	m_LightMat->TexUpdate();
	m_LightMesh->Update();
	m_LightMesh->Render();
	m_LightMat->TexReset();
}

void HLight::SetType(LIGHTTYPE _Type) 
{
	m_eType = _Type;

	switch (_Type)
	{
	case HLight::DIR:
		m_LightMesh = ResourceManager<HMesh>::Find(L"RECT");
		break;
	case HLight::POINT:
		m_VolumMesh = ResourceManager<HMesh>::Find(L"SPHERE");
		m_LightMesh = ResourceManager<HMesh>::Find(L"RECT");		
		break;
	case HLight::SPOT:
		break;
	default:
		break;
	}

	m_VolumeMat = ResourceManager<HMaterial>::Find(L"VOLUMEMAT");
	m_LightMat = ResourceManager<HMaterial>::Find(L"DEFFERDLIGHTMAT");
}