#pragma once
#include "Component_DE.h"
#include "KMath.h"
#include <set>
#include <functional>

// 도형을 표현할때.
// 도형은 방정식이다.
// 위치를 표현하는 X Y
// +X +Y
// 0 == X(+Y)
// 원
// 사각형
// 점

// 상대 점이
// 방정식은 항상 X Y == 0
// KVector4 하나만 있다면 뭘 표현할수 있는가?

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
	CT_AABB3D, // 1// 회전 하지 않는 큐브
	CT_OBB3D,// 회전 하지 않는 큐브
	CT_MAX
};

class HColCom;
class HColFiFunc;
class HColFi 
{
public:
	friend HColCom;
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

class HColFiFunc
{
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
};


class KCore;
class HCol2DMgr;
class HColCom : public Component_DE
{
public:
	friend KCore;
	friend HCol2DMgr;

private:
	static void ColInit();
	static bool(*ColFunc[CT_MAX][CT_MAX])(const HColFi* _Left, const  HColFi* _Right);

private:
	HColFi* m_Fi;

protected:
	int	    m_Order;

private:
	std::set<HColCom*>::iterator ColFindIter;
	std::set<HColCom*> m_ColSet;

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
	void ColCheck(HColCom* _Col);
	bool FiColCheck(const HColFi* _Col);

private:
	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>>::iterator m_EnterStartIter;
	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>>::iterator m_EnterEndIter;
	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>> m_EnterFuncList;

	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>>::iterator m_StayStartIter;
	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>>::iterator m_StayEndIter;
	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>> m_StayFuncList;

	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>>::iterator m_ExitStartIter;
	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>>::iterator m_ExitEndIter;
	std::list<std::function<void(HColCom* _Left, HColCom* _Right)>> m_ExitFuncList;

public:
	// 함수를 호출할때 그 오브젝트가 지워진다면 터지게 된다.
	template<typename T>
	void EnterFunc(T* _Obj, void(T::*_PTR)(HColCom*, HColCom*))
	{
		if (nullptr == _PTR)		{			return;		}
		if (nullptr == _Obj)		{			return;		}
		m_EnterFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void StayFunc(T* _Obj, void(T::*_PTR)(HColCom*, HColCom*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_StayFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void ExitFunc(T* _Obj, void(T::*_PTR)(HColCom*, HColCom*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_ExitFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

private:
	void CallEnterList(HColCom* _Right);
	void CallStayList(HColCom* _Right);
	void CallExitList(HColCom* _Right);

public:
	HColCom();
	virtual ~HColCom();
};

