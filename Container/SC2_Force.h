#pragma once
#include <Begin_Updater.h>
#include <DXContainer.h>
#include <map>


class Force_Unit;
class SC2_Force : public Begin_Updater
{
public:
	friend class SC2Manager;

private:
	enum PLAYER_TYPE
	{
		PRT_NONE,
		PRT_PC,
		PRT_PERSON,
	};

private:
	KColor m_Color;
	
	std::map<std::wstring, KPtr<Force_Unit>> m_UMap;
	std::map<std::wstring, KPtr<Force_Unit>>::iterator m_SUI;
	std::map<std::wstring, KPtr<Force_Unit>>::iterator m_EUI;


private:
	bool Init();
	void Update();

public:
	KUINT units_size()
	{
		return (KUINT)m_UMap.size();
	}

	KPtr<Force_Unit> Find_Unit(const wchar_t* _Name)
	{
		return m_UMap.find(_Name)->second;
	}


	template<typename T>
	KPtr<Force_Unit> Create_Unit(const wchar_t* _Name)
	{
		if (0 == _Name[0])
		{
			return nullptr;
		}

		KPtr<T> TU = new T(_Name);

		TU->Init();
		m_UMap.insert(std::map<std::wstring, KPtr<Force_Unit>>::value_type(_Name, TU));

		return TU;
	}

public:
	SC2_Force(const wchar_t* _Name, const KColor& _Color);
	~SC2_Force();
};

