#include "Collision.h"
#include "TransPosition.h"


void Collision::Init_Collision()
{
	for (size_t y = 0; y < COL_MAX; y++)
	{
		for (size_t x = 0; x < COL_MAX; x++)
		{
			Collision_Act[y][x] = nullptr;
		}
	}

	// 이런식으로 입력
	Collision_Act[COL_RECT2D][COL_RECT2D] = &Collision_Calc::Rect2Rect;
	Collision_Act[COL_RECT2D][COL_POINT2D] = &Collision_Calc::Rect2Point;
	Collision_Act[COL_RECT2D][COL_CIRCLE2D] = &Collision_Calc::Rect2Circle;
	Collision_Act[COL_CIRCLE2D][COL_CIRCLE2D] = &Collision_Calc::Circle2Circle;
	Collision_Act[COL_CIRCLE2D][COL_POINT2D] = &Collision_Calc::Circle2Point;
	Collision_Act[COL_CIRCLE2D][COL_RECT2D] = &Collision_Calc::Circle2Rect;
	Collision_Act[COL_POINT2D][COL_CIRCLE2D] = &Collision_Calc::Point2Circle;
	Collision_Act[COL_POINT2D][COL_RECT2D] = &Collision_Calc::Point2Rect;

	Collision_Act[COL_POLY2D][COL_RECT2D] = &Collision_Calc::Rect2Rect;
	Collision_Act[COL_POLY2D][COL_POINT2D] = &Collision_Calc::Rect2Point;
	Collision_Act[COL_POLY2D][COL_CIRCLE2D] = &Collision_Calc::Rect2Circle;

	Collision_Act[COL_RECT2D][COL_POLY2D] = &Collision_Calc::Rect2Rect;
	Collision_Act[COL_POINT2D][COL_POLY2D] = &Collision_Calc::Point2Rect;
	Collision_Act[COL_CIRCLE2D][COL_POLY2D] = &Collision_Calc::Circle2Rect;
	Collision_Act[COL_POLY2D][COL_POLY2D] = &Collision_Calc::Rect2Rect;
}

bool(*Collision::Collision_Act[COL_MAX][COL_MAX])(Collider_Figure* _Left, Collider_Figure* _Right,
	TransPosition* _LeftPo, TransPosition* _RightPo);

Collision::Collision() : Col_Check(false)
{
}

Collision::~Collision()
{
	if (nullptr != m_Figure)
	{
		delete m_Figure;
	}
}

void Collision::check(Collision* _Other)
{
	if (nullptr == Collision_Act)
	{
		return;
	}

	Col_Check = collision_check(_Other);

	// 충돌시
	if (true == Col_Check)
	{
		F_Collision_Iter = m_Collsion_Set.find(_Other);

		// 리스트에 없다 - 엔터
		if (F_Collision_Iter == m_Collsion_Set.end())
		{
			// 여기서 상호 참조 - 실제 안나기 위해서 그냥 포인터로 한 거임
			m_Collsion_Set.insert(_Other);
			_Other->m_Collsion_Set.insert(this);
			Enter(_Other);
			_Other->Enter(this);
		}
		// 있다. - 스테이
		else
		{
			Stay(_Other);
			_Other->Stay(this);
		}
	}

	// 충돌 아닐 시
	else
	{
		F_Collision_Iter = m_Collsion_Set.find(_Other);
		
		// 리스트에ㅔ 있다 - Exit >> 나머지는 아예 부딪히지 않는 것이므로생략ㅇㅇ
		// 그냥 API와 같닼ㅋ
		if (F_Collision_Iter != m_Collsion_Set.end())
		{
			m_Collsion_Set.erase(_Other);
			_Other->m_Collsion_Set.erase(this);
			Exit(_Other);
			_Other->Exit(this);
		}
	}
	
	return ;
}


bool Collision::collision_check(Collision* _Other)
{
	if (nullptr == _Other)
	{
		return false;
	}

	if (_Other->type() == COL_MAX || m_Figure->m_type == COL_MAX)
	{
		return false;
	}

	TransPosition* Test1 = one()->transform();
	TransPosition* Test2 = _Other->one()->transform();

	return Collision_Act[m_Figure->m_type][_Other->m_Figure->m_type](m_Figure, _Other->m_Figure,
		one()->transform(), _Other->one()->transform());
}

bool Collision::Init()
{
	return true;
}

void Collision::Update_Collision()
{

}



void Collision::Enter(Collision* _Other)
{
	m_SEnter_Iter = m_EnterList.begin();
	m_EEnter_Iter = m_EnterList.end();

	for (; m_SEnter_Iter != m_EEnter_Iter; ++m_SEnter_Iter)
	{
		(*m_SEnter_Iter)(this, _Other);
	}
}


void Collision::Stay(Collision* _Other)
{
	m_SStay_Iter = m_StayList.begin();
	m_EStay_Iter = m_StayList.end();

	for (; m_SStay_Iter != m_EStay_Iter; ++m_SStay_Iter)
	{
		(*m_SStay_Iter)(this, _Other);
	}
}


void Collision::Exit(Collision* _Other)
{
	m_SExit_Iter = m_ExitList.begin();
	m_EExit_Iter = m_ExitList.end();

	for (; m_SExit_Iter != m_EExit_Iter; ++m_SExit_Iter)
	{
		(*m_SExit_Iter)(this, _Other);
	}
}



void Collision::death_release()
{
	std::set<Collision*>::iterator StartIter = m_Collsion_Set.begin();
	std::set<Collision*>::iterator EndIter = m_Collsion_Set.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->m_Collsion_Set.erase(this);
	}
}