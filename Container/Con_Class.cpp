#include "Con_Class.h"



KPtr<SC2Manager> Con_Class::m_S2Manager = nullptr;

Con_Class::Con_Class()
{
}


Con_Class::~Con_Class()
{
}


KPtr<SC2Manager> Con_Class::s2_manager()
{
	if (nullptr == m_S2Manager)
	{
		m_S2Manager = new SC2Manager();
	}

	return m_S2Manager;
}