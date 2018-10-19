#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
class KWindow;
class HWindowBase
{
public:
	friend class HSceneMgr;
	friend class HActor;
	friend class HScene;

private:
	KWindow* m_pWindow;

public:
	KWindow* Window() 
	{
		return m_pWindow;
	}

private:
	void Window(KWindow* _pWindow)
	{
		m_pWindow = _pWindow;
	}

protected:
	HWindowBase(KWindow* _pWindow) : m_pWindow(_pWindow) {}
	HWindowBase() : m_pWindow(nullptr) {}
	~HWindowBase() {}
};

