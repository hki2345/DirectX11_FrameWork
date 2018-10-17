#include "HLight.h"
#include "HScene.h"
#include "HResMgr.h"
#include "HVAR.h"
#include "HMesh.h"
#include "HMaterial.h"
#include "GameDebug.h"


HLight::HLight() : m_eType(LIGHTTYPE::DIR)
{
	Data.Lc.Diff = HVEC(1.0f, 1.0f, 1.0f, 1.0f);
	Data.Lc.Spec = HVEC(0.2f, 0.2f, 0.2f, 1.0f);
	Data.Lc.Ambi = HVEC(0.1f, 0.1f, 0.1f, 1.0f);
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

void HLight::CalLightData(HPTR<HCamera> _Camera) {
	Data.LightDir = Trans()->WForward();
	Data.LightPos = Trans()->WPos();
	Data.Type = m_eType;
	Data.Range = Trans()->WScale().x;

	//wchar_t Arr[256];
	//swprintf_s(Arr, L"LightDir : %f, %f, %f", Data.LightDir.x, Data.LightDir.y, Data.LightDir.z);
	//GameDebug::DrawLog(Arr);
	//swprintf_s(Arr, L"LightPos : %f, %f, %f", Data.LightPos.x, Data.LightPos.y, Data.LightPos.z);
	//GameDebug::DrawLog(Arr);

	Data.LightDir = _Camera->CV().MulVecZero(Data.LightDir);
	Data.LightDirInv = -Data.LightDir;
	Data.LightPos = _Camera->CV().MulVecOne(Data.LightPos);
}

void HLight::LightRender(HPTR<HCamera> _Camera)
{
	// 라이트 데이터
	HVAR::MainDevice().SetBsMode(L"ONE");
	HVAR::MainDevice().SettingCB<LightData>(L"DEFFERDLIGHTDATA", Data, SHTYPE::ST_PS);

	if (nullptr == m_LightMat || nullptr == m_LightMesh)
	{
		BOOM;
	}

	if (m_eType == LIGHTTYPE::POINT)
	{
		HMAT Scale;
		Scale.Scale(HVEC(m_Trans->WScale().x, m_Trans->WScale().x, m_Trans->WScale().x));
		HMAT World;
		World.Iden();
		World = Scale * m_Trans->CRMat() * m_Trans->CPMat();

		m_MatData.m_W = World.RVTranspose();
		m_MatData.m_V = _Camera->CV().RVTranspose();
		m_MatData.m_P = _Camera->CP().RVTranspose();
		m_MatData.m_WV = (World * _Camera->CV()).RTranspose();
		m_MatData.m_WVP = (World * _Camera->VP()).RTranspose();

		HVAR::MainDevice().SettingCB<MATDATA>(L"MATDATA", m_MatData, SHTYPE::ST_VS);
		HVAR::MainDevice().SettingCB<MATDATA>(L"MATDATA", m_MatData, SHTYPE::ST_PS);

		// 두번해야 하느냐?
		// 후면보다 앞에 있는 픽셀 검색.
		HVAR::MainDevice().SetRsMode(L"SFRONT");
		HVAR::MainDevice().SetDsMode(L"BACK_ST", 1);
		m_VolumeMat->Update();
		m_VolumMesh->Update();
		m_VolumMesh->Render();

		HVAR::MainDevice().SetRsMode(L"SBACK");
		HVAR::MainDevice().SetDsMode(L"FRONT_ST", 1);
		m_VolumeMat->Update();
		m_VolumMesh->Update();
		m_VolumMesh->Render();
		HVAR::MainDevice().SetDsMode(L"PASS_ST", 1);
	}
	else if (m_eType == LIGHTTYPE::SPOT)
	{

	}
	else 
	{
		HVAR::MainDevice().SetDsMode(L"PASS_ST", 0);
	}

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
		m_LightMesh = HResMgr<HMesh>::Find(L"RECT");
		break;
	case HLight::POINT:
		m_VolumMesh = HResMgr<HMesh>::Find(L"SPHERE");
		m_LightMesh = HResMgr<HMesh>::Find(L"RECT");		
		break;
	case HLight::SPOT:
		break;
	default:
		break;
	}

	m_VolumeMat = HResMgr<HMaterial>::Find(L"VOLUMEMAT");
	m_LightMat = HResMgr<HMaterial>::Find(L"DEFFERDDIRMAT");
}