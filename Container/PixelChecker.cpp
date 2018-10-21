#include "PixelChecker.h"
#include <State.h>
#include <InputManager.h>
#include <DebugManager.h>
#include <HSpRenderer.h>


PixelChecker::PixelChecker()
{
}


PixelChecker::~PixelChecker()
{


}

bool PixelChecker::Init() 
{
	SpriteRender = Get_Component<HSpRenderer>();
	return true;
}

void PixelChecker::DebugRender() 
{
	KVector2 MPos = state()->Camera()->ScreenTo_World(InputManager::MousePos());

	KVector4 Color = SpriteRender->Image()->texture()->GetPixel(262, 82);

	wchar_t Arr[256];
	swprintf_s(Arr, L"MWorldPos : R %d G %d B %d A %d", Color.ir, Color.ig, Color.ib, Color.ia);
	DebugManager::Draw_Font(Arr, { 10.0f, 300.0f }, 20.0f);
}