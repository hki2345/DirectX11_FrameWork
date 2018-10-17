#pragma once
#include "HCol2DBase.h"

class HCol2D : public HCol2DBase
{
private:
	Base2DColFi* m_2DCol;

public:
	bool Init(int _Order = 0);
	void ColFiUpdate() override;
	void Mode(COLTYPE _Type);
	void Radius(HVEC2 _Size)
	{
		m_SizeSelf = true;
		m_2DCol->m_Vec.m_Size = _Size;
	}
	void Radius(float _Radius) 
	{
		m_SizeSelf = true;
		m_2DCol->m_Vec.m_Vec3.Radius = _Radius;
	}

public:
	void DebugRender() override;

public:
	HCol2D();
	~HCol2D();
};

