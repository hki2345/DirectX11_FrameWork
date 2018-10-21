#pragma once
#include "SomeOne.h"

class PixelChecker : public SomeOne
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

