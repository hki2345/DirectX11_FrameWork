#pragma once
#include "SC2Manager.h"


class Con_Class
{
private:
	static KPtr<SC2Manager> m_S2Manager;


public:
	static void s2_manager(KPtr<SC2Manager> _Manager)
	{
		m_S2Manager = _Manager;
	}

	static KPtr<SC2Manager> s2_manager()
	{
		if (nullptr == m_S2Manager)
		{
			m_S2Manager = new SC2Manager();
		}

		return m_S2Manager;
	}




public:
	Con_Class();
	~Con_Class();
};

