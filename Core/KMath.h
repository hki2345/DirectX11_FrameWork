#pragma once
#include "DXContainer.h"

class KMath
{
public:

public:
	static bool RectToRect(const KRect& _Left, const KRect& _Right)
	{ 
		if (_Left.left() > _Right.right())
		{
			return false;
		}

		if (_Left.right() < _Right.left())
		{
			return false;
		}

		if (_Left.top() < _Right.bottom())
		{
			return false;
		}

		if (_Left.bottom() > _Right.top())
		{
			return false;
		}

		return true; 
	}
	static bool RectToPoint(const KRect& _Left, const KPoint& _Right) 
	{
		if (_Left.left() > _Right.x)
		{
			return false;
		}

		if (_Left.right() < _Right.x)
		{
			return false;
		}

		if (_Left.top() < _Right.y)
		{
			return false;
		}

		if (_Left.bottom() > _Right.y)
		{
			return false;
		}

		return true;
	}

	static bool CirCleToCirCle(const KCircle& _Left, const KCircle& _Right)
	{
		if (_Left.Radius + _Right.Radius > (_Left.m_Pos - _Right.m_Pos).length())
		{
			return true;
		}
		return false;
	}
	static bool CirCleToPoint(const KCircle& _Left, const KPoint& _Right)
	{
		if (_Left.Radius > (_Left.m_Pos - _Right).length())
		{
			return true;
		}
		return false;
	}

	static KRect GKRect;
	static KRect GWRECT;
	static KCircle GCILCLE;
	static bool RectToCirCle(const KRect& _Left, const KCircle& _Right)
	{
		GKRect = _Left;
		GKRect.m_Size.y += _Right.diameter();
		GWRECT = _Left;
		GWRECT.m_Size.x += _Right.diameter();
		if (true == RectToPoint(GKRect, _Right.m_Pos))
		{
			return true;
		}

		if (true == RectToPoint(GWRECT, _Right.m_Pos))
		{
			return true;
		}

		KVector2 Rt = _Left.right_top();
		Rt -= _Left.m_Pos;
		GCILCLE = _Right;
		GCILCLE.m_Pos -= _Left.m_Pos;
		GCILCLE.m_Pos.x = fabsf(GCILCLE.m_Pos.x);
		GCILCLE.m_Pos.y = fabsf(GCILCLE.m_Pos.y);

		if (true == CirCleToPoint(GCILCLE, Rt))
		{
			return true;
		}

		return false;
	}


	float DegToRad(const float& _Value)
	{
		return _Value * KPI / 180.0f;
	}

	float RadToDeg(const float& _Value)
	{
		return _Value * 180.0f / KPI;
	}


public:
	KMath();
	~KMath();
};

