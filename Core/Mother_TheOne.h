#pragma once
// �������̽� �� -> ���߻���� ���� ��������� ���... ����߰� ����
// ���� ����


class TheOne;
class Mother_TheOne
{
private:
	TheOne* m_pOne;

public:
	TheOne* one()
	{
		return m_pOne;
	}

	void one(TheOne* _pOne)
	{
		m_pOne = _pOne;
	}

protected:
	Mother_TheOne() :m_pOne(nullptr) {};
	~Mother_TheOne() {};
};

