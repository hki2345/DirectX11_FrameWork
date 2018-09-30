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

	// ���带 ��Ű�� ������ ������� ���� �ƴ�
	// nullptr�� �Ǿ� ������ ���ϰ� �ϴ� ���� -> ����Ʈ �����Ϳ���
	// ������ �� ��� ����
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

