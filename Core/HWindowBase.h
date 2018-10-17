#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
class HWindow;
class HWindowBase
{
public:
	friend class HSceneMgr;
	friend class HActor;
	friend class HScene;

private:
	HWindow* m_pWindow;

public:
	HWindow* Window() 
	{
		return m_pWindow;
	}

private:
	void Window(HWindow* _pWindow)
	{
		m_pWindow = _pWindow;
	}

protected:
	HWindowBase(HWindow* _pWindow) : m_pWindow(_pWindow) {}
	HWindowBase() : m_pWindow(nullptr) {}
	~HWindowBase() {}
};

