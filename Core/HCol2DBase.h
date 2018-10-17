#pragma once
#include "HColCom.h"

class HCol2DBase : public HColCom
{
protected:
	bool m_SizeSelf;

	HVEC2 Pivot;
	HVEC2 Size;

public:
	void PosSetting();
	void SizeSetting();

public:
	HCol2DBase();
	~HCol2DBase();
};

