#include "PixelChecker.h"
#include <HScene.h>
#include <InputMgr.h>
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
	SpriteRender = GetCom<HSpRenderer>();
	return true;
}

void PixelChecker::DebugRender() 
{
	KVector2 MPos = Scene()->Camera()->ScreenToWorld(InputMgr::MousePos());

	KVector4 Color = SpriteRender->Image()->GetTex()->GetPixel(262, 82);

	wchar_t Arr[256];
	swprintf_s(Arr, L"MWorldPos : R %d G %d B %d A %d", Color.ir, Color.ig, Color.ib, Color.ia);
	DebugManager::DrawFont(Arr, { 10.0f, 300.0f }, 20.0f);
}