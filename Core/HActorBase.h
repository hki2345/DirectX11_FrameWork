#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
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

