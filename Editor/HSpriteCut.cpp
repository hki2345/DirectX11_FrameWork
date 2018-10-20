#include "stdafx.h"
#include "HSpriteCut.h"
#include <DebugManager.h>

HSpriteCut::HSpriteCut()
{
}


HSpriteCut::~HSpriteCut()
{
}

bool HSpriteCut::Init() 
{
	SpriteRender = AddCom<HSpRenderer>();

	return true;
}

void HSpriteCut::DebugRender() 
{
	if (nullptr == SpriteRender->Image())
	{
		return;
	}
	
	KVector2 ImageCount = SpriteRender->Image()->CutIndex();

	KVector2 ImageSize = SpriteRender->Image()->GetTex()->ImageSize();

	KVector2 RectSize;

	RectSize.x = ImageSize.x / ImageCount.x;
	RectSize.y = ImageSize.y / ImageCount.y;


	KVector2 PivotPos = { -ImageSize.x * 0.5f + RectSize.x * 0.5f, ImageSize.y * 0.5f - RectSize.y * 0.5f };
	KVector2 RectPos;


	//if (0 >= Size.stx || 0 >= Size.sty)
	//{
	//	return;
	//}

	//SpriteRender->Image()->Uv();

	for (size_t y = 0; y < (size_t)ImageCount.y; y++)
	{
		for (size_t x = 0; x < (size_t)ImageCount.x; x++)
		{
			RectPos = KVector2{ x * RectSize.x, y * -RectSize.y };

			RectPos += PivotPos;

			// KRect(x * 100, y * 100, (x + 1) * 100, (y + 1) * 100);

			DebugManager::DrawRect(KRect(RectPos.x, RectPos.y, RectSize.x, RectSize.y));
		}
	}


}