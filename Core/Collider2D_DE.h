#pragma once
#include "Collider2D.h"
#include <vector>

class Collider2D_DE : public Collider2D
{
private:
	Figure_2D*	m_Fig2D;
	KColor		m_Color;

public:
	bool Init(const int& _Order = 0);
	void Update_Collision() override;
	void DebugRender() override;

	void mode(const COLL_TYPE& _type)
	{
		switch (_type)
		{
		case COL_POINT2D:
		case COL_RECT2D:
		case COL_CIRCLE2D:
		case COL_POLY2D:
			m_Fig2D->m_type = _type;
			break;
		default:
			m_Fig2D->m_type = COLL_TYPE::COL_RECT2D;
			break;
		}
	}

	void color(const KColor& _Value) { m_Color = _Value; }
	KColor color() const { return m_Color; }
	
	void Radius(const KVector2& _Value)
	{
		m_ind_Scale = true;
		m_Fig2D->m_Vector.m_Size = _Value;
	}
	void Radius(const float& _Value)
	{
		m_ind_Scale = true;
		m_Fig2D->m_Vector.m_Vector3.Radius = _Value;
	}

public:
	Collider2D_DE();
	~Collider2D_DE();
};

