#pragma once
// 인터페이스 형 -> 다중상속을 위해 헤더꼬임을 고려... 헤더추가 없음
// 구현 없음


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

