#pragma once
// 인터페이스 형 -> 다중상속을 위해 헤더꼬임을 고려... 헤더추가 없음
// 구현 없음


class State;
class Mother_State
{
	// 스테이트는 당연히 자신을 셋팅할 줄 ㅇㄹ아야하지 않겠는가
public:
	friend State;

private:
	State* m_pState;

public:
	State* state()
	{
		return m_pState;
	}

	void state(State* _pState)
	{
		m_pState = _pState;
	}

protected:
	Mother_State() :m_pState(nullptr) {};
	~Mother_State() {};
};

