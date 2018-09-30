#include "Collider2D_DE.h"
#include "State.h"
#include "DebugManager.h"



Collider2D_DE::Collider2D_DE() : m_Color ( .0f, 1.0f, .0f, 1.0f )
{
	m_Fig2D = create_figure<Figure_2D>(COLL_TYPE::COL_RECT2D);
}


Collider2D_DE::~Collider2D_DE()
{
}

bool Collider2D_DE::Init(const int& _Order/* = 0*/)
{
	m_Order = _Order;
	state()->this_Collider2DManager.insert_2D(this);
	return true;
}

void Collider2D_DE::Update_Collision()
{
	m_Fig2D->m_Vector.m_Pos = m_Pivot + m_Trans->World_Pos();

	if (false == m_ind_Scale)
	{
		switch (m_Fig2D->m_type)
		{
		case COL_RECT2D:
			m_Fig2D->m_Vector.m_Size.set_vector2(fabsf(m_Trans->World_Scale_Const().x), fabsf(m_Trans->World_Scale_Const().y));
			break;
		case COL_CIRCLE2D:
			m_Fig2D->m_Vector.m_Vector3.Radius = fabsf(m_Trans->World_Scale_Const().x) * .5f;
			break;
		case COL_POLY2D:
			break;
		default:
			break;
		}
	}
	else
	{
		switch (m_Fig2D->m_type)
		{
		case COL_RECT2D:
			m_Fig2D->m_Vector.m_Size.set_vector2(fabsf(m_Trans->World_Scale_Const().x), fabsf(m_Trans->World_Scale_Const().y));
			break;
		case COL_CIRCLE2D:
			m_Fig2D->m_Vector.m_Vector3.Radius = fabsf(m_Trans->World_Scale_Const().x) * .5f;
			break;
		case COL_POLY2D:
			break;
		default:
			break;
		}
	}
}

void Collider2D_DE::DebugRender()
{
//#ifdef _DEBUG
//	DebugManager::draw_rect( one()->transform(), m_Fig2D->m_type, 5.0f, m_Color);
//#endif // _DEBUG
}
