#pragma once
// �������̽� �� -> ���߻���� ���� ��������� ���... ����߰� ����
// ���� ����


class State;
class Mother_State
{
	// ������Ʈ�� �翬�� �ڽ��� ������ �� �����ƾ����� �ʰڴ°�
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

