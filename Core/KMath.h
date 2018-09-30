#pragma once
#include "DirectXHeader.h"
#include <vector>

class TransPosition;
class KMath
{
public:
	static const float DegToRad;
	static const float RadToDeg;

public:
	static bool EpsiolonCheckf(const float& _Value)
	{
		if (fabsf(_Value) < .00000001f)
		{
			return true;
		}

		return false;
	}
	static float EpsiolonCutf(float& _Value)
	{
		if (fabsf(_Value) < .00000001f)
		{
			_Value = .0f;
		}

		return _Value;
	}



	static bool Rect2Rect(TransPosition* _LTrans, TransPosition* _RTrans);
	static bool Circle2Circle(const KCircle& _Left, const KCircle& _Right);
	static bool Circle2Point(const KCircle& _Left, const KPoint& _Right);

	// Vec 1 -> Up, Vec 2 -> Right, Vec 3 -> Pos, Vec 4 -> Scale
	// const  가 안붙는 이유는 여기서 막 값이 변하며 검출하기 때문
	static bool Rect2Point(KMatrix& _LTrans, KMatrix& _RTrans);
	static bool Rect2Circle(TransPosition* _LTrans, TransPosition* _RTrans);

	static bool Poly2Point(TransPosition* _LTrans, TransPosition* _RTrans);
	static bool Poly2Circle(TransPosition* _LTrans, TransPosition* _RTrans);
	static bool Poly2Rect(TransPosition* _LTrans, TransPosition* _RTrans);
	static bool Poly2Poly(TransPosition* _LTrans, TransPosition* _RTrans);

	static KVector4 CenterOfGravity(TransPosition* _LTrans);
	static float Area(std::vector<KVector2>* _LVector);

private:
	static bool CheckOBB(KVector _T, const KMatrix& _LTRans, const KMatrix& _RTRans);
	static bool CheckOBB(KVector _T, TransPosition* _LTrans, TransPosition* _RTrans);

	// 폴리건 체크 - 궁극의 충돌 ㅋㅋㅋㅋㅋㅋ
	static bool Poly2PolyOBB(TransPosition* _LTrans, TransPosition* _RTrans);
	static float TofVector2(const KVector2& _T, std::vector<KVector2> _Value);

public:
	KMath();
	~KMath();
};

