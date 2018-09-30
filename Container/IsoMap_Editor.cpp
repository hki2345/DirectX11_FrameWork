#include "IsoMap_Editor.h"
#include <State.h>
#include <InputManager.h>


IsoMap_Editor::IsoMap_Editor()
{
}


IsoMap_Editor::~IsoMap_Editor()
{
}


bool IsoMap_Editor::Init()
{
	m_IsoRender = Get_Component<IsoMap_Renderer>();

	return true;
}
void IsoMap_Editor::Update()
{
	KVector2 MPos = state()->camera()->screen_to_world(InputManager::mouse_pos());

	if (KEY_PRESS("MouseButton"))
	{
		m_IsoRender->create_tile(MPos, 1);
	}
}
void IsoMap_Editor::DebugRender()
{
}