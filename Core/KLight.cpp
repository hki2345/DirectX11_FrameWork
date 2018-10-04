#include "KLight.h"
#include "State.h"
#include "DebugManager.h"

KLight::KLight()
{
	m_LD.LC.Diff = KVector(1.0f, 1.0f, 1.0f, 1.0f);
	m_LD.LC.Spec = KVector(.2f, .2f, .2f, 1.0f);
	m_LD.LC.Ambi = KVector(.1f, .1f, .1f, 1.0f);
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

	WLOG(L"Light CamPos: %f, %f, %f", m_LD.CamPos.x, m_LD.CamPos.y, m_LD.CamPos.z);
}