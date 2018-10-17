#pragma once
#include "DHeader.h"

class GameMath
{
public:

public:
	static bool RectToRect(const HRECT& _Left, const HRECT& _Right)
	{ 
		if (_Left.FLEFT() > _Right.FRIGHT())
		{
			return false;
		}

		if (_Left.FRIGHT() < _Right.FLEFT())
		{
			return false;
		}

		if (_Left.FTOP() < _Right.FBOT())
		{
			return false;
		}

		if (_Left.FBOT() > _Right.FTOP())
		{
			return false;
		}

		return true; 
	}
	static bool RectToPoint(const HRECT& _Left, const HPOINT& _Right) 
	{
		if (_Left.FLEFT() > _Right.x)
		{
			return false;
		}

		if (_Left.FRIGHT() < _Right.x)
		{
			return false;
		}

		if (_Left.FTOP() < _Right.y)
		{
			return false;
		}

		if (_Left.FBOT() > _Right.y)
		{
			return false;
		}

		return true;
	}

	static bool CirCleToCirCle(const HCIRCLE& _Left, const HCIRCLE& _Right)
	{
		if (_Left.Radius + _Right.Radius > (_Left.m_Pos - _Right.m_Pos).Len())
		{
			return true;
		}
		return false;
	}
	static bool CirCleToPoint(const HCIRCLE& _Left, const HPOINT& _Right)
	{
		if (_Left.Radius > (_Left.m_Pos - _Right).Len())
		{
			return true;
		}
		return false;
	}

	static HRECT GHRECT;
	static HRECT GWRECT;
	static HCIRCLE GCILCLE;
	static bool RectToCirCle(const HRECT& _Left, const HCIRCLE& _Right)
	{
		GHRECT = _Left;
		GHRECT.m_Size.y += _Right.Diameter();
		GWRECT = _Left;
		GWRECT.m_Size.x += _Right.Diameter();
		if (true == RectToPoint(GHRECT, _Right.m_Pos))
		{
			return true;
		}

		if (true == RectToPoint(GWRECT, _Right.m_Pos))
		{
			return true;
		}

		HVEC2 Rt = _Left.RT();
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


public:
	GameMath();
	~GameMath();
};

