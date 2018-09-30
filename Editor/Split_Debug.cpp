#include "stdafx.h"
#include "Split_Debug.h"
#include <DebugManager.h>

Split_Debug::Split_Debug() : m_Renderer(nullptr)
{
}


Split_Debug::~Split_Debug()
{
}


bool Split_Debug::Init()
{
	m_Renderer = Add_Component<Renderer_Sprite>();
	
	return true;
}

void Split_Debug::DebugRender()
{
	if (nullptr == m_Renderer->image())
	{
		return;
	}

	KVector2 ImgCnt = m_Renderer->image()->index();
	KVector2 ImgSize = m_Renderer->image()->texture()->size();

	KVector2 RtSize;

	RtSize.x = ImgSize.x / ImgCnt.x;
	RtSize.y = ImgSize.y / ImgCnt.y;

	KVector2 pivot = { (-ImgSize.x + RtSize.x) * .5f, (ImgSize.y - RtSize.y) * .5f };
	KVector2 RtPos;

	for (size_t y = 0; y < (size_t)ImgCnt.y; y++)
	{
		for (size_t x = 0; x < (size_t)ImgCnt.x; x++)
		{
			RtPos = KVector2(x * RtSize.x, y* -RtSize.y);
			RtPos += pivot;

			DebugManager::draw_rect(RtPos, RtSize, {1.0f, 1.0f, .0f, 1.0f}, 3);
		}
	}
}