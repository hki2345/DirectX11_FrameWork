#pragma once
#include <SomeOne.h>
#include <HSpRenderer.h>

class HSpriteCut : public SomeOne
{
public:
	KPtr<HSpRenderer> SpriteRender;


public:
	bool Init() override;
	void DebugRender() override;


public:
	HSpriteCut();
	~HSpriteCut();
};

