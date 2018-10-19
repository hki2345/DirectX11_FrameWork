#pragma once
#include "Begin.h"

class Begin_Updater : public Begin
{
private:
	bool m_bUpdate;
	bool m_bDeath;

public:
	virtual bool Is_Active()
	{
		return false == m_bDeath && true == m_bUpdate;
	}

	virtual bool Is_Death()
	{
		return true == m_bDeath;
	}

	virtual void Set_Death()
	{
		m_bDeath = true;
	}



	virtual void Active_On()
	{
		m_bUpdate = true;
	}

	virtual void Active_Off()
	{
		m_bUpdate = false;
	}

public:
	virtual bool Init();
	virtual void PrevUpdate();
	virtual void Update();
	virtual void NextUpdate();
	virtual void DebugUpdate();
	virtual void DebugRender();

protected:
	Begin_Updater();
	~Begin_Updater();
};

