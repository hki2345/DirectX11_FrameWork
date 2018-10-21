#include "Renderer_Rect.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"


Renderer_Rect::Renderer_Rect()
{
}


Renderer_Rect::~Renderer_Rect()
{
}
void Renderer_Rect::Render(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate();

	m_MD.m_W = m_Trans->worldmat_const();
	m_MD.m_V = _Camera->View();
	m_MD.m_P = _Camera->Proj();
	m_MD.m_WVP = (CSWMat() * _Camera->ViewProj()).RTranspose();

	m_Mat->VShader()->SettingCB<MatrixContainer>(L"MATCON", m_MD);

	m_Mat->Update();
	m_Mesh->Update();
	m_Mesh->Render();
}

bool Renderer_Rect::Init(int _Order /*= 0*/) 
{
	Renderer::Init(_Order);

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
