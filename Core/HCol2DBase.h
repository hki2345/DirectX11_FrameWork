#pragma once
#include "HColCom.h"

class HCol2DBase : public HColCom
{
protected:
	bool m_SizeSelf;

	KVector2 Pivot;
	KVector2 Size;

public:
	void PosSetting();
	void SizeSetting();

public:
	HCol2DBase();
	~HCol2DBase();
};

