#include "SC2_Force.h"







SC2_Force::SC2_Force(const wchar_t* _Name, const KColor& _Color)
{
	name(_Name);
	memcpy_s(m_Con.Name, NAMENUM, _Name, NAMENUM);
	this->m_Con.Color = _Color;
}


SC2_Force::~SC2_Force()
{
}


KPtr<Force_Unit> SC2_Force::Find_Unit(const wchar_t* _Name)
{
	m_SUI = m_UMap.find(_Name);

	if (m_UMap.end() == m_SUI)
	{
		return nullptr;
	}

	return m_SUI->second;
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
		m_SUI->second->Update();
	}
}