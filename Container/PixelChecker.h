#pragma once
#include "HLogic.h"

class PixelChecker : public HLogic
{
private:
	KPtr<class HSpRenderer> SpriteRender;

public:
	bool Init() override;
	void DebugRender() override;

public:
	PixelChecker();
	~PixelChecker();
};

