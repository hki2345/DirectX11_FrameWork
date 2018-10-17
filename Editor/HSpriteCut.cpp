#include "stdafx.h"
#include "HSpriteCut.h"
#include <GameDebug.h>

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
	
	HVEC2 ImageCount = SpriteRender->Image()->CutIndex();

	HVEC2 ImageSize = SpriteRender->Image()->GetTex()->ImageSize();

	HVEC2 RectSize;

	RectSize.x = ImageSize.x / ImageCount.x;
	RectSize.y = ImageSize.y / ImageCount.y;


	HVEC2 PivotPos = { -ImageSize.x * 0.5f + RectSize.x * 0.5f, ImageSize.y * 0.5f - RectSize.y * 0.5f };
	HVEC2 RectPos;


	//if (0 >= Size.stx || 0 >= Size.sty)
	//{
	//	return;
	//}

	//SpriteRender->Image()->Uv();

	for (size_t y = 0; y < (size_t)ImageCount.y; y++)
	{
		for (size_t x = 0; x < (size_t)ImageCount.x; x++)
		{
			RectPos = HVEC2{ x * RectSize.x, y * -RectSize.y };

			RectPos += PivotPos;

			// HRECT(x * 100, y * 100, (x + 1) * 100, (y + 1) * 100);

			GameDebug::DrawRect(HRECT(RectPos.x, RectPos.y, RectSize.x, RectSize.y));
		}
	}


}