#include "SC2_Force.h"
#include "Force_Unit.h"



SC2_Force::SC2_Force(const wchar_t* _Name, const KColor& _Color)
{
	Begin::name(_Name);
	this->m_Color = _Color;
}


SC2_Force::~SC2_Force()
{
}


bool SC2_Force::Init()
{
	return true;
}


void SC2_Force::Update()
{
	m_SUI = m_UMap.begin();
	m_EUI = m_UMap.begin();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		if (true == m_SUI->second->Is_Active())
		{
			m_SUI->second->Update();
		}
	}
}