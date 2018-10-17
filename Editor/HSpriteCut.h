#pragma once
#include <HLogic.h>
#include <HSpRenderer.h>

class HSpriteCut : public HLogic
{
public:
	HPTR<HSpRenderer> SpriteRender;


public:
	bool Init() override;
	void DebugRender() override;


public:
	HSpriteCut();
	~HSpriteCut();
};

