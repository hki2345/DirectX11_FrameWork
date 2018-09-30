#pragma once
#include <Some_One.h>
#include <Renderer_Sprite.h>
#include <Collider2D_DE.h>

class Split_Debug : public Some_One
{
public:
	KPtr<Renderer_Sprite> m_Renderer;

public:
	bool Init() override;
	void DebugRender() override;

public:
	Split_Debug();
	~Split_Debug();
};

