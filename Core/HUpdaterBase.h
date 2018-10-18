#pragma once
#include "Begin.h"

class HUpdaterBase : public Begin
{
private:
	bool m_bUpdate;
	bool m_bDeath;

public:
	virtual bool IsUpdate()
	{
		return false == m_bDeath && true == m_bUpdate;
	}

	virtual void On()
	{
		m_bUpdate = true;
	}

	virtual void Off()
	{
		m_bUpdate = false;
	}

	virtual bool IsDeath()
	{
		return true == m_bDeath;
	}

	virtual void Death()
	{
		m_bDeath = true;
	}

public:
	virtual bool Init();
	virtual void PrevUpdate();
	virtual void Update();
	virtual void NextUpdate();
	virtual void DebugUpdate();
	virtual void DebugRender();

protected:
	HUpdaterBase();
	~HUpdaterBase();
};

