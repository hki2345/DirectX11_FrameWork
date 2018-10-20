#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
class KWindow;
class Mof_KWindow
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
	Mof_KWindow(KWindow* _pWindow) : m_pWindow(_pWindow) {}
	Mof_KWindow() : m_pWindow(nullptr) {}
	~Mof_KWindow() {}
};

