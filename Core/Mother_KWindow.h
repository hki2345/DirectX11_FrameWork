#pragma once
// �������̽� �� -> ���߻���� ���� ��������� ���... ����߰� ����
// ���� ����


class KWindow;
class Mother_KWindow
{
public:
	friend class StateManager;
	friend class State;
	friend class TheOne;


private:
	KWindow* m_pWindow;

public:
	KWindow* window()
	{
		return m_pWindow;
	}

	void window(KWindow* _pWindow)
	{
		m_pWindow = _pWindow;
	}

protected:
	Mother_KWindow(KWindow* _Window) :m_pWindow(_Window) {};
	Mother_KWindow() :m_pWindow(nullptr){};
	~Mother_KWindow() {};
};

