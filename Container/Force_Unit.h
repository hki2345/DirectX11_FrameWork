#pragma once
#include <SomeOne.h>

class SC2_Force;
class Force_Unit : public SomeOne
{
	// ������ �Ӽ��� �����Ǵ� ����̱� ������ �� ������� �ϴ� �� �´�.
	// �� ��� - ��Ʈ���� ����
private:
	int m_UnitType;


public:
	Force_Unit(const wchar_t* _Name);
	~Force_Unit();
};

