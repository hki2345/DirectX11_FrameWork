#pragma once
#include "Component_DE.h"
#include "KMath.h"
#include <set>
#include <list>
#include <functional>


enum COLL_TYPE
{
	COL_POINT2D,
	COL_RECT2D,
	COL_CIRCLE2D,
	COL_POLY2D,
	COL_PLANE2D,

	COL_POINT3D,
	COL_SPHERE3D,
	COL_RAY3D,
	COL_AABB3D,
	COL_OBB3D,
	COL_MAX,
};

// 2D �� 3D�� �浹�� �������� �ʹ� ���� ������
// ������. -> �׸��� �ؾ��� �ǹ��� ������ ���� -> �׳� �ʹ� ����.

class Collision;
class Collision_Calc;
class Collider_Figure
{
public:
	friend Collision;
	friend Collision_Calc;

public:
	COLL_TYPE m_type;

protected:
	Collider_Figure() : m_type(COL_MAX){}
	virtual ~Collider_Figure() = 0 {}
};


class Figure_2D : public Collider_Figure 
{
public:
	KVector4 m_Vector;

public:
	Figure_2D(){}
	virtual ~Figure_2D(){}
};


class Collision_Calc
{
public:
	static bool Rect2Rect(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);

	static bool Circle2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);

	static bool Point2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);

	static bool Point2Rect(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);


	static bool Rect2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);

	static bool Circle2Point(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo)
	{
		return Point2Circle(_Right, _Left, _RightPo, _LeftPo);
	}

	static bool Rect2Point(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo)
	{
		return Point2Rect(_Right, _Left, _RightPo, _LeftPo);
	}

	static bool Circle2Rect(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo)
	{
		return Rect2Circle(_Right, _Left, _RightPo, _LeftPo);
	}



	static bool Rect2Poly(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo)
	{
		return Poly2Rect(_Right, _Left, _RightPo, _LeftPo);
	}
	static bool Circle2Poly(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo)
	{
		return Poly2Circle(_Right, _Left, _RightPo, _LeftPo);
	}
	static bool Point2Poly(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo)
	{
		return Poly2Point(_Right, _Left, _RightPo, _LeftPo);
	}

	static bool Poly2Rect(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);
	static bool Poly2Circle(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);
	static bool Poly2Point(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);
	static bool Poly2Poly(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo);
};


class KCore;
class Collider2DManager;
class TransPosition;
class Collision : public Component_DE
{
public:
	friend KCore;
	friend Collider2DManager;

private:
	bool Col_Check;

	static void Init_Collision();

	// �Լ� ������ - �� �迭�� ��� ����
	// ���� ����
	// void(*�̸�)(); -> �̋� void �� ��ȯ�ϴ� �μ��� ���� ���� ����Ѵ�.
	// ���ڸ� �ؼ��� �ִ� ���� �ִµ� functional�� ����Ѵ�. -> �����ð��� �������� ���ٴ�.
	static bool(*Collision_Act[COL_MAX][COL_MAX])(Collider_Figure* _Left, Collider_Figure* _Right,
		TransPosition* _LeftPo, TransPosition* _RightPo );

private:
	bool collision_check(Collision* _Other);

	std::set<Collision*>::iterator F_Collision_Iter;
	std::set<Collision*> m_Collsion_Set;

public:
	// ����� ���� �ۺ����� �Ѵ�. ����
	Collider_Figure* m_Figure;


protected:
	int m_Order;

public:
	int& order() { return m_Order; }

	void death_release();
	virtual bool Init();
	virtual void Update_Collision();

	bool is_collision() 
	{
		return Col_Check; 
	}


protected:
	template<typename T>
	T* create_figure(const COLL_TYPE& _type)
	{
		T* NewFi = new T();
		NewFi->m_type = _type;
		m_Figure = NewFi;
		return NewFi;
	}
	
public:
	void check(Collision* _Other);

	COLL_TYPE& type() const { return m_Figure->m_type; }
	void type(const COLL_TYPE& _Value) { m_Figure->m_type = _Value; }

private:
	// �Լ� .... ������ 
	// void ���� ���� 2���� �޴� �Լ� �����͸� ����Ʈ�� ��� �ִ� ���̴�.
	// �̰� ���� �浹�� �Ǹ� this�� _Other�� �־ �����Ű�� �ǰڴ�.
	std::list<std::function<void(Collision* _Left, Collision* _Right)>>::iterator m_SEnter_Iter;
	std::list<std::function<void(Collision* _Left, Collision* _Right)>>::iterator m_EEnter_Iter;
	std::list<std::function<void(Collision* _Left, Collision* _Right)>> m_EnterList;


	std::list<std::function<void(Collision* _Left, Collision* _Right)>>::iterator m_SStay_Iter;
	std::list<std::function<void(Collision* _Left, Collision* _Right)>>::iterator m_EStay_Iter;
	std::list<std::function<void(Collision* _Left, Collision* _Right)>> m_StayList;

	std::list<std::function<void(Collision* _Left, Collision* _Right)>>::iterator m_SExit_Iter;
	std::list<std::function<void(Collision* _Left, Collision* _Right)>>::iterator m_EExit_Iter;
	std::list<std::function<void(Collision* _Left, Collision* _Right)>> m_ExitList;

public:
	// ��... �ش� ����ü�� �ı��ǰų� --�θ� �� TheOne�� collider�� ��ٰ�  �ı��� ���
	// ������� ������ ����.
	template<typename T>
	void enter_insert(T* _Obj, void(T::*_PTR)(Collision*, Collision*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }

		// �� �ִ� ��� ����Ʈ ���ڸ� �ָ�-
		// �Լ�, ��ü, ���� --- ���� �� �����Ŵ� 
		m_EnterList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}
	template<typename T>
	void stay_insert(T* _Obj, void(T::*_PTR)(Collision*, Collision*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }

		m_StayList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}
	template<typename T>
	void exit_insert(T* _Obj, void(T::*_PTR)(Collision*, Collision*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }

		m_ExitList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

private:
	void Enter(Collision* _Other);
	void Stay(Collision* _Other);
	void Exit(Collision* _Other);

public:
	Collision();
	virtual ~Collision();
};

