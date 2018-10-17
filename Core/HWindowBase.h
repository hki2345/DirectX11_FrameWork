#pragma once
// 이안에 다른 헤더가 없다.
// 무조건 구현해주게 하고 싶을때.
// c#에서는 이런걸 인터페이스라고 한다.
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

