#include "KMath.h"
#include "DirectXHeader.h"
#include "TransPosition.h"

const float KMath::DegToRad = KPI / 180.0f;
const float KMath::RadToDeg = 180.0f / KPI;

KMath::KMath()
{
}


KMath::~KMath()
{
}


// ���� ���� , L���� , R����
bool KMath::CheckOBB(KVector _T, TransPosition* _LTrans, TransPosition* _RTrans)
{
	float dis = fabsf(_T.Dot(_LTrans->Local_Pos() - _RTrans->Local_Pos()));

	// �ش� ���� ���Ϳ� ���� ��Ų(�׸���) ���Ͱ� �� �������� ���� ����(�׸���)���� 
	// Ŀ���� �� ���� ���Ϳ� ���ؼ� �浹�ϴ� ����̴�.
	// �̰��� ��ü 2���� ���� �� �鿡 �ϳ��� �����ϴ� ��� -> 4*4�� �ƴ� ������ ������ ���ʹ� ������ �ʿ� ����
	if (dis >	
		fabsf(_T.Dot(_LTrans->Local_Up() * _LTrans->Local_Scale().y * .5f)) +
		fabsf(_T.Dot(_LTrans->Local_Right() * _LTrans->Local_Scale().x * .5f)) +
		fabsf(_T.Dot(_RTrans->Local_Up() * _RTrans->Local_Scale().y * .5f)) +
		fabsf(_T.Dot(_RTrans->Local_Right() * _RTrans->Local_Scale().x * .5f)))
	{
		return false;
	}


	return true;
}

// ���� ��Ʈ �浹
bool KMath::CheckOBB(KVector _T, const KMatrix& _LTRans, const KMatrix& _RTRans)
{
	float dis = fabsf(_T.Dot(_LTRans.vec3 - _RTRans.vec3));
	// �ش� ���� ���Ϳ� ���� ��Ų(�׸���) ���Ͱ� �� �������� ���� ����(�׸���)���� 
	// Ŀ���� �� ���� ���Ϳ� ���ؼ� �浹�ϴ� ����̴�.
	// �̰��� ��ü 2���� ���� �� �鿡 �ϳ��� �����ϴ� ��� -> 4*4�� �ƴ� ������ ������ ���ʹ� ������ �ʿ� ����
	if (dis >	fabsf(_T.Dot(_LTRans.vec1 * _LTRans.vec4.y * .5f)) +
				fabsf(_T.Dot(_LTRans.vec2 * _LTRans.vec4.x * .5f)) +
				fabsf(_T.Dot(_RTRans.vec1 * _RTRans.vec4.y * .5f)) +
				fabsf(_T.Dot(_RTRans.vec2 * _RTRans.vec4.x * .5f)))
	{
		return false;
	}

	return true;
}


bool KMath::Rect2Rect(TransPosition* _LTrans, TransPosition* _RTrans)
{
	if (CheckOBB(_LTrans->Local_Up(), _LTrans, _RTrans) &&
		CheckOBB(_RTrans->Local_Up(), _LTrans, _RTrans) && 
		CheckOBB(_LTrans->Local_Right(), _LTrans, _RTrans) && 
		CheckOBB(_RTrans->Local_Right(), _LTrans, _RTrans))
	{
		return true;
	}
	return false;
}

bool KMath::Rect2Point(KMatrix& _LTrans, KMatrix&  _RTrans)
{
	if (CheckOBB(_LTrans.vec1, _LTrans, _RTrans) &&
		CheckOBB(_RTrans.vec1, _LTrans, _RTrans) &&
		CheckOBB(_LTrans.vec2, _LTrans, _RTrans) &&
		CheckOBB(_RTrans.vec2, _LTrans, _RTrans))
	{
		return true;
	}
	return false;
}

bool KMath::Circle2Circle(const KCircle& _Left, const KCircle& _Right)
{
	if (_Left.Radius + _Right.Radius > (_Left.m_Pos - _Right.m_Pos).length())
	{
		return true;
	}
	return false;
}

bool KMath::Circle2Point(const KCircle& _Left, const KPoint& _Right)
{
	if (_Left.Radius * .5f > (_Left.m_Pos - _Right).length())
	{
		return true;
	}
	return false;
}




KMatrix TempLMat;
KMatrix TempRMat;
KCircle temp_circle;
bool KMath::Rect2Circle(TransPosition* _LTrans, TransPosition* _RTrans)
{
	TempLMat.vec1 = _LTrans->Local_Up();
	TempLMat.vec2 = _LTrans->Local_Right();
	TempLMat.vec3 = _LTrans->Local_Pos();
	TempLMat.vec4 = _LTrans->Local_Scale();
	TempLMat.vec4.x += _RTrans->Local_Scale().x;

	TempRMat.vec1 = KVector(.0f, 1.0f, .0f, .0f);
	TempRMat.vec2 = KVector(1.0f, .0f, .0f, .0f);;
	TempRMat.vec3 = _RTrans->Local_Pos();
	TempRMat.vec4 = KVector(1.0f, 1.0f, 1.0f, .0f);

	if (true == Rect2Point(TempLMat, TempRMat))
	{
		// ����Ʈ�� �� �浹����
		return true;
	}

	TempLMat.vec4 = _LTrans->Local_Scale();
	TempLMat.vec4.y += _RTrans->Local_Scale().y;

	if (true == Rect2Point(TempLMat, TempRMat))
	{
		// ����Ʈ�� �� �浹����
		return true;
	}

	temp_circle = KCircle(_RTrans->Local_Pos().x, _RTrans->Local_Pos().y, _RTrans->Local_Scale().x * .5f);


	// OBB ���� �� �� �������� �� Ȯ���ؾ��Ѵ�. -> ��ǥ�� �ٴٸ��Ƿ�
	if (true == Circle2Point(temp_circle, _LTrans->Local_RightTop()) || 
		true == Circle2Point(temp_circle, _LTrans->Local_RightBottom()) || 
		true == Circle2Point(temp_circle, _LTrans->Local_LeftTop()) || 
		true == Circle2Point(temp_circle, _LTrans->Local_LeftBottom()))
	{
		// �� ������ �� �ϳ��� �浹����
		return true;
	}

	return false;
}


bool KMath::Poly2Point(TransPosition* _LTrans, TransPosition* _RTrans)
{
	if (true == Poly2PolyOBB(_LTrans, _RTrans))
	{
		return true;
	}
	return false;
}
bool KMath::Poly2Circle(TransPosition* _LTrans, TransPosition* _RTrans)
{
	return false;
}
bool KMath::Poly2Rect(TransPosition* _LTrans, TransPosition* _RTrans)
{
	if (true == Poly2PolyOBB(_LTrans, _RTrans))
	{
		return true;
	}
	return false;
}
bool KMath::Poly2Poly(TransPosition* _LTrans, TransPosition* _RTrans)
{
	if (true == Poly2PolyOBB(_LTrans, _RTrans))
	{
		return true;
	}
	return false;
}

// �ñ��� Polygon üũ
bool KMath::Poly2PolyOBB(TransPosition* _LTrans, TransPosition* _RTrans)
{
	return false;
	if (nullptr == _LTrans->Col_Vertex() || nullptr == _RTrans->Col_Vertex())
	{
	}

	std::vector<KVector2> LVec = *_LTrans->Col_Vertex();
	std::vector<KVector2> RVec = *_RTrans->Col_Vertex();

	KVector2 MainVec = (_LTrans->Local_Pos() - _RTrans->Local_Pos()).m_Pos;
	

	for (size_t i = 0; i < LVec.size(); i++)
	{
		LVec[i] += _LTrans->Local_Pos().m_Pos;
	}
	for (size_t i = 0; i < RVec.size(); i++)
	{
		RVec[i] += _RTrans->Local_Pos().m_Pos;
	}


	// L���� ������
	for (size_t i = 0; i < LVec.size(); i++)
	{
		KVector2 T;
		if (i != LVec.size() - 1)
		{
			T = LVec[i + 1] - LVec[i];
		}
		else if (i == LVec.size() - 1)
		{
			T = LVec[0] - LVec[i];
		}
		T.normalize();

		float Test = TofVector2(T, LVec);
		float Test1 = TofVector2(T, RVec);
		float Distance = fabsf(T.dot(MainVec));
		if (Distance < (TofVector2(T, LVec) + TofVector2(T, RVec)) * .5f)
		{
			return false;
		}
	}

	// R���� ������
	for (size_t i = 0; i < RVec.size(); i++)
	{
		KVector2 T;
		if (i != RVec.size() - 1)
		{
			T = RVec[i + 1] - RVec[i];
		}
		else if (i == RVec.size() - 1)
		{
			T = RVec[0] - RVec[i];
		}
		T.normalize();

		float Distance = fabsf(T.dot(MainVec));
		if (Distance < (TofVector2(T, LVec) + TofVector2(T, RVec)) * .5f)
		{
			return false;
		}
	}

	return true;
}

float KMath::TofVector2(const KVector2& _T, std::vector<KVector2> _Value)
{
	float Edge = 0;
	for (size_t i = 0; i < _Value.size(); i++)
	{
		Edge += fabsf(_T.dot(_Value[i]));
	}

	// �������� ���� �ΰ��� ������ �����ϸ鼭 ��ġ�� ������ ���ǹ����� ���������
	return Edge * .25f;
}


KVector4 KMath::CenterOfGravity(TransPosition* _Trans)
{
	if (nullptr == _Trans->Col_Vertex())
	{
		return KVector2::Zero;
	}

	std::vector<KVector2> Temp = *_Trans->Col_Vertex();

	float VecArea = Area(_Trans->Col_Vertex());
	KVector2 COG = KVector2::Zero;


	for (size_t i = 0; i < Temp.size() - 1; i++)
	{
		COG.x += (Temp[i].x + Temp[i + 1].x) * ((Temp[i].x * Temp[i + 1].y) - (Temp[i + 1].x * Temp[i].y));
		COG.y += (Temp[i].y + Temp[i + 1].y) * ((Temp[i].x * Temp[i + 1].y) - (Temp[i + 1].x * Temp[i].y));
	}

	COG /= 6.0f * VecArea;

	return COG;
}


float KMath::Area(std::vector<KVector2>* _LVector)
{
	if (nullptr == _LVector)
	{
		return 0;
	}

	std::vector<KVector2> Temp = *_LVector;
	float area = 0;
	for (size_t i = 0; i < Temp.size() - 1; i++)
	{
		area += (Temp[i].x * Temp[i + 1].y) - (Temp[i + 1].x * Temp[i].y);
	}

	return  area * .5f;
}