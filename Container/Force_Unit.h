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
	// ������ �Ӽ��� �����Ǵ� ����̱� ������ �� ������� �ϴ� �� �´�.
	// �� ��� - ��Ʈ���� ����
private:
	KPtr<SC2_Force> m_Con;
	int m_UnitType;


public:
	Force_Unit(const wchar_t* _Name, KPtr<SC2_Force> _Force);
	~Force_Unit();
};

