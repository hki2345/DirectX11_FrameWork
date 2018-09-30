#include "Collision.h"
#include "TransPosition.h"


bool Collision_Calc::Rect2Rect(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
#ifdef _DEBUG
	//if (nullptr == _Left || nullptr == _Right)
	//{
	//	return false;
	//}
	//if (_Left->m_type != COLL_TYPE::COL_RECT2D || _Right->m_type != COLL_TYPE::COL_RECT2D)
	//{
	//	return false;
	//}
#endif
	return KMath::Rect2Rect(_LeftPo, _RightPo);
}

bool Collision_Calc::Circle2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
#ifdef _DEBUG
	//if (nullptr == _Left || nullptr == _Right)
	//{
	//	return false;
	//}
	//if (_Left->m_type != COLL_TYPE::COL_CIRCLE2D || _Right->m_type != COLL_TYPE::COL_CIRCLE2D)
	//{
	//	return false;
	//}
#endif
	return KMath::Circle2Circle(
		((Figure_2D*)_Left)->m_Vector.m_Vector3,
		((Figure_2D*)_Right)->m_Vector.m_Vector3);
}

bool Collision_Calc::Point2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
#ifdef _DEBUG
	/*if (nullptr == _Left || nullptr == _Right)
	{
		return false;
	}
	if (_Left->m_type != COLL_TYPE::COL_POINT2D || _Right->m_type != COLL_TYPE::COL_CIRCLE2D)
	{
		return false;
	}*/
#endif
	return KMath::Circle2Point(
		KCircle(_RightPo->Local_Pos().m_Pos, _RightPo->Local_Scale().x),
		KPoint(_LeftPo->Local_Pos().m_Pos));
}

bool Collision_Calc::Point2Rect(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
#ifdef _DEBUG
	/*if (nullptr == _Left || nullptr == _Right)
	{
		return false;
	}
	if (_Left->m_type != COLL_TYPE::COL_POINT2D || _Right->m_type != COLL_TYPE::COL_RECT2D )
	{
		if (_Right->m_type != COLL_TYPE::COL_POLY2D)
		{
			return false;
		}
	}*/
#endif

	KMatrix TempLMat;
	TempLMat.vec1 = _LeftPo->Local_Up();
	TempLMat.vec2 = _LeftPo->Local_Right();
	TempLMat.vec3 = _LeftPo->Local_Pos();
	TempLMat.vec4 = 1.0f;

	KMatrix TempRMat;
	TempRMat.vec1 = _RightPo->Local_Up();
	TempRMat.vec2 = _RightPo->Local_Right();
	TempRMat.vec3 = _RightPo->Local_Pos();
	TempRMat.vec4 = _RightPo->Local_Scale();

	return KMath::Rect2Point(TempRMat, TempLMat);
}

bool Collision_Calc::Rect2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
#ifdef _DEBUG
	/*if (nullptr == _Left || nullptr == _Right)
	{
		return false;
	}
	if (_Left->m_type != COLL_TYPE::COL_RECT2D || _Right->m_type != COLL_TYPE::COL_CIRCLE2D)
	{
		return false;
	}*/
#endif
	return KMath::Rect2Circle(_LeftPo, _RightPo);
}


bool Collision_Calc::Poly2Rect(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
	return KMath::Poly2Rect(_LeftPo, _RightPo);
}
bool Collision_Calc::Poly2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
	return  KMath::Poly2Circle(_LeftPo, _RightPo);
}
bool Collision_Calc::Poly2Point(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
	return  KMath::Poly2Point(_LeftPo, _RightPo);
}

bool Collision_Calc::Poly2Poly(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo)
{
	return  KMath::Poly2Poly(_LeftPo, _RightPo);
}