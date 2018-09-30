#pragma once
#include "Begin.h"

class Begin_Updater : public Begin
{
private:
	bool m_b_Active;
	bool m_b_Death;

public:
	bool is_Active()
	{
		return false == m_b_Death && true == m_b_Active;
	}

	void active_On()
	{
		m_b_Active = true;
	}

	void active_Off()
	{
		m_b_Active = false;
	}

	// 데드를 시키면 실제로 사라지는 것이 아닌
	// nullptr로 되어 접근을 못하게 하는 형식 -> 스마트 포인터에서
	// 지워질 떄 모두 없앰
	bool is_Death()
	{
		return true == m_b_Death;
	}

	virtual void set_Death()
	{
		m_b_Death = true;
	}

public:
	virtual bool Init();
	virtual void PrevUpdate();
	virtual void Update();
	virtual void NextUpdate();
	virtual void UIRender();
	virtual void DebugUpdate();
	virtual void DebugRender();

protected:
	Begin_Updater();
	~Begin_Updater();
};

