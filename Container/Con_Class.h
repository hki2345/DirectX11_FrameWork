#pragma once
#include "SC2Manager.h"


class Con_Class
{
private:
	static KPtr<SC2Manager> m_S2Manager;


public:
	static KPtr<SC2Manager> s2_manager();




public:
	Con_Class();
	~Con_Class();
};

