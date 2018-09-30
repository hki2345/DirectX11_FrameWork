#pragma once
#include "Collision.h"
class Collider2D : public Collision
{
protected:
	bool m_ind_Scale;

	KVector2 m_Pivot;
	KVector2 m_Scale;

public:
	void set_pos();
	void set_scale();

public:
	Collider2D();
	~Collider2D();
};

