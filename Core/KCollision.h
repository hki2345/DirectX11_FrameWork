#pragma once
#include "Component_DE.h"
#include "KMath.h"
#include <set>
#include <functional>

// ������ ǥ���Ҷ�.
// ������ �������̴�.
// ��ġ�� ǥ���ϴ� X Y
// +X +Y
// 0 == X(+Y)
// ��
// �簢��
// ��

// ��� ����
// �������� �׻� X Y == 0
// KVector4 �ϳ��� �ִٸ� �� ǥ���Ҽ� �ִ°�?

enum COLTYPE
{
	CT_POINT2D, // 8
	CT_RECT2D, // 7
	CT_CIRCLE2D, // 6
	CT_POLY2D, // 4
	CT_PLANE3D,
	CT_POINT3D,
	CT_SPHERE3D, // 3
	CT_RAY3D, // 2
	CT_AABB3D, // 1// ȸ�� ���� �ʴ� ť��
	CT_OBB3D,// ȸ�� ���� �ʴ� ť��
	CT_MAX
};

class KCollision;
class HColFiFunc;
class HColFi 
{
public:
	friend KCollision;
	friend HColFiFunc;

public:
	COLTYPE m_ColType;

public:
	HColFi() : m_ColType(CT_MAX) {}
	virtual ~HColFi() = 0 {	}
};

class Base2DColFi : public HColFi
{
public:
	KVector4 m_Vec;

public:
	Base2DColFi() {}
	virtual ~Base2DColFi() {}
};



class KSphereCon : public HColFi
{
public:
	DirectX::BoundingSphere m_Sphere;
public:
	KSphereCon() {}
	virtual ~KSphereCon() {}
};

class KRayCon : public HColFi
{
public:
	KVector Ori;
	KVector Dir;
	float Dist;

public:
	KRayCon() {}
	virtual ~KRayCon() {}
};




class HColFiFunc
{

#pragma region Funtion 2D
public:
	static bool RectToRectFi(const HColFi* _Left, const HColFi* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_RECT2D || _Right->m_ColType != COLTYPE::CT_RECT2D)
		{
			return false;
		}
#endif
		return KMath::RectToRect(((Base2DColFi*)_Left)->m_Vec, ((Base2DColFi*)_Right)->m_Vec);
	}
	static bool CirCleToCirCleFi(const HColFi* _Left, const HColFi* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_CIRCLE2D || _Right->m_ColType != COLTYPE::CT_CIRCLE2D)
		{
			return false;
		}
#endif
		return KMath::CirCleToCirCle(((Base2DColFi*)_Left)->m_Vec.m_Vec3, ((Base2DColFi*)_Right)->m_Vec.m_Vec3);
	}

	static bool PointToCirCleFi(const HColFi* _Left, const  HColFi* _Right) {
		return CirCleToPointFi(_Right, _Left);
	}
	static bool CirCleToPointFi(const HColFi* _Left, const  HColFi* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_CIRCLE2D || _Right->m_ColType != COLTYPE::CT_POINT2D)
		{
			return false;
		}
#endif
		return KMath::CirCleToPoint(((Base2DColFi*)_Left)->m_Vec.m_Vec3, ((Base2DColFi*)_Right)->m_Vec.m_Pos);
	}

	static bool PointToRectFi(const HColFi* _Left, const  HColFi* _Right) {
		return RectToPointFi(_Right, _Left);
	}
	static bool RectToPointFi(const HColFi* _Left, const HColFi* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_RECT2D || _Right->m_ColType != COLTYPE::CT_POINT2D)
		{
			return false;
		}
#endif
		return KMath::RectToPoint(((Base2DColFi*)_Left)->m_Vec, ((Base2DColFi*)_Right)->m_Vec.m_Pos);
	}

	static bool CirCleToRectFi(const HColFi* _Left, const HColFi* _Right) {
		return RectToCirCleFi(_Right, _Left);
	}

	static bool RectToCirCleFi(const HColFi* _Left, const  HColFi* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_RECT2D || _Right->m_ColType != COLTYPE::CT_CIRCLE2D)
		{
			return false;
		}
#endif
		return KMath::RectToCirCle(((Base2DColFi*)_Left)->m_Vec, ((Base2DColFi*)_Right)->m_Vec.m_Vec3);
	}

#pragma endregion

	static bool SphereToSphereFunc(const HColFi* _Left, const HColFi* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_SPHERE3D || _Right->m_ColType != COLTYPE::CT_SPHERE3D)
		{
			return false;
		}
#endif
		return KMath::SphereToSphere(((KSphereCon*)_Left)->m_Sphere, ((KSphereCon*)_Right)->m_Sphere);
	}

	static bool SphereToRayFunc(const HColFi* _Left, const HColFi* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_SPHERE3D || _Right->m_ColType != COLTYPE::CT_RAY3D)
		{
			return false;
		}
#endif
		return KMath::SphereToRay(((KSphereCon*)_Left)->m_Sphere, ((KRayCon*)_Right)->Ori, ((KRayCon*)_Right)->Dir, ((KRayCon*)_Right)->Dist);

	}

	static bool RayToSphereFunc(const HColFi* _Left, const HColFi* _Right)
	{
		return SphereToRayFunc(_Right, _Left);
	}

};


class KCore;
class CollisionManager;
class KCollision : public Component_DE
{
public:
	friend KCore;
	friend CollisionManager;

private:
	static void ColInit();
	static bool(*ColFunc[CT_MAX][CT_MAX])(const HColFi* _Left, const  HColFi* _Right);

private:
	HColFi* m_Fi;

protected:
	int	    m_Order;

private:
	std::set<KCollision*>::iterator ColFindIter;
	std::set<KCollision*> m_ColSet;

public:
	void DeathRelease();

public:
	virtual bool Init();
	virtual void ColFiUpdate();

protected:
	template<typename T>
	T* CreateColFi(COLTYPE _ColType)
	{
		T* Fi = new T();
		Fi->m_ColType = _ColType;
		m_Fi = Fi;
		return Fi;
	}

public:
	void ColCheck(KCollision* _Col);
	bool FiColCheck(const HColFi* _Col);

private:
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_EnterStartIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_EnterEndIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>> m_EnterFuncList;

	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_StayStartIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_StayEndIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>> m_StayFuncList;

	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_ExitStartIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_ExitEndIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>> m_ExitFuncList;

public:
	// �Լ��� ȣ���Ҷ� �� ������Ʈ�� �������ٸ� ������ �ȴ�.
	template<typename T>
	void EnterFunc(T* _Obj, void(T::*_PTR)(KCollision*, KCollision*))
	{
		if (nullptr == _PTR)		{			return;		}
		if (nullptr == _Obj)		{			return;		}
		m_EnterFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void StayFunc(T* _Obj, void(T::*_PTR)(KCollision*, KCollision*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_StayFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void ExitFunc(T* _Obj, void(T::*_PTR)(KCollision*, KCollision*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_ExitFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

private:
	void CallEnterList(KCollision* _Right);
	void CallStayList(KCollision* _Right);
	void CallExitList(KCollision* _Right);

public:
	KCollision();
	virtual ~KCollision();
};
