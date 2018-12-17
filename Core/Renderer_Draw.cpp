#include "Renderer_Draw.h"

#include "KWindow.h"
#include "InputManager.h"

#include "KPlane3D_Col.h"



Renderer_Draw::Renderer_Draw()
{
}


Renderer_Draw::~Renderer_Draw()
{
}

bool Renderer_Draw::Init(const int& _Order /*= 0*/)
{
	Renderer::Init(_Order);

	if (false == Set_Material(L"DRAWMTL"))
	{
		return false;
	}

	material()->PShader()->CreateCB<Draw_Info>(L"DRAW_IF", D3D11_USAGE_DYNAMIC, 0);
	Set_RSState(L"SBACK");


	m_Col = one()->Get_Component<KCollider3D_DE>();
	return true;
}

void Renderer_Draw::Update_Trans(KPtr<Camera> _Camera)
{
	Renderer::Update_Trans(_Camera);
}


void Renderer_Draw::Update()
{
	if (true == KEY_PRESS("MouseButton"))
	{
		m_DI.OnClick = 1;
	}
	else
	{
		m_DI.OnClick = 0;
	}

	if (nullptr == m_Col)
	{
		m_Col = one()->Get_Component<KPlane3D_Col>();
		return;
	}


	m_DI.MousePos = KMath::PostoUV2(m_Col->figure()->m_ColPoint, m_Trans);
	m_DI.PPUV = KVector2(1 / m_Trans->scale_local().x, 1 / m_Trans->scale_local().y);

	KLOG(L"Mouse UV: %f, %f", m_DI.MousePos.x, m_DI.MousePos.y);
}

void Renderer_Draw::RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	material()->PShader()->SettingCB<Draw_Info>(L"DRAW_IF", m_DI);
}
