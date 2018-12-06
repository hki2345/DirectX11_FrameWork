#pragma once
#include <SomeOne.h>

//
//class Unit_Container : public KResource
//{
//
//};



class SC2_Force;
class Force_Unit : public SomeOne
{
	// 유닛의 속성은 누적되는 방식이기 때문에 이 방식으로 하는 게 맞다.
	// 이 방식 - 비트단위 연산
private:
	KPtr<SC2_Force> m_Con;
	int m_UnitType;


public:
	Force_Unit(const wchar_t* _Name, KPtr<SC2_Force> _Force);
	~Force_Unit();
};

