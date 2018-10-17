#pragma once
// 이안에 다른 헤더가 없다.
// 무조건 구현해주게 하고 싶을때.
// c#에서는 이런걸 인터페이스라고 한다.
class HActor;
class HActorBase
{
public:
	friend class HActor;
	friend class HComponent;

private:
	HActor* m_pActor;

public:
	HActor* Actor()
	{
		return m_pActor;
	}

private:
	void Actor(HActor* _pWindow)
	{
		m_pActor = _pWindow;
	}

protected:
	HActorBase() : m_pActor(nullptr) {}
	~HActorBase() {}
};

