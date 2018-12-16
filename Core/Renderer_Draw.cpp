#include "Renderer_Draw.h"

#include "KWindow.h"
#include "InputManager.h"



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

	// m_DI.MousePos = InputManager::MousePos();
}

void Renderer_Draw::RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	material()->PShader()->SettingCB<Draw_Info>(L"DRAW_IF", m_DI);
}
