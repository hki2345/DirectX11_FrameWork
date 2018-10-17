#pragma once
#include "HLogic.h"

class PixelChecker : public HLogic
{
private:
	HPTR<class HSpRenderer> SpriteRender;

public:
	bool Init() override;
	void DebugRender() override;

public:
	PixelChecker();
	~PixelChecker();
};

