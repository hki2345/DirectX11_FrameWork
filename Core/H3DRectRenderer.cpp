#include "H3DRectRenderer.h"
#include "KMacro.h"
#include "HWindow.h"
#include "HTrans.h"


H3DRectRenderer::H3DRectRenderer()
{
}


H3DRectRenderer::~H3DRectRenderer()
{
}
void H3DRectRenderer::Render(KPtr<HCamera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate();

	m_MatData.m_W = m_Trans->CWMat();
	m_MatData.m_V = _Camera->CV();
	m_MatData.m_P = _Camera->CP();
	m_MatData.m_WVP = (CSWMat() * _Camera->VP()).RTranspose();

	m_Mat->VTXSH()->SettingCB<MATDATA>(L"MATDATA", m_MatData);

	m_Mat->Update();
	m_Mesh->Update();
	m_Mesh->Render();
}

bool H3DRectRenderer::Init(int _Order /*= 0*/) 
{
	HRenderer::Init(_Order);

	if (false == SetMat(L"RECT3DMAT"))
	{
		return false;
	}
	if (false == SetMesh(L"RECT"))
	{
		return false;
	}

	SetRSState(L"SBACK");
	return true;
}
