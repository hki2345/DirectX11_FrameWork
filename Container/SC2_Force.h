#pragma once
// 유닛도 세력을 알아야하지만 유닛은 간접적으로 아는 식(포인터)
// 요놈은 유닛을 직접 만들어야 하니까 값으로 알아야 겠다.
#include "Force_Unit.h"
#include <DXContainer.h>
#include "Container.h"

#include <map>

// 이름과 그 세력의 고유 색을 가진다
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
	std::map<std::wstring, KPtr<Force_Unit>> m_UMap;
	std::map<std::wstring, KPtr<Force_Unit>>::iterator m_SUI;
	std::map<std::wstring, KPtr<Force_Unit>>::iterator m_EUI;

	Force_Container m_Con;

private:
	bool Init();
	void Update();

public:

	KColor& force_color()
	{
		return m_Con.Color;
	}

	wchar_t*  force_name()
	{
		return m_Con.Name;
	}

	void force_color(const KColor& _Value)
	{
		m_Con.Color = _Value;
	}

	void  force_name(const wchar_t* _Value)
	{
		memcpy_s(m_Con.Name, sizeof(wchar_t) * NAMENUM, _Value, sizeof(wchar_t) * NAMENUM);
	}



	Force_Container* force_container()
	{
		return &m_Con;
	}

	bool Set_Force(const Force_Container& _Value)
	{
		name(_Value.Name);
		memcpy_s(m_Con.Name, sizeof(wchar_t) * NAMENUM, _Value.Name, sizeof(wchar_t) * NAMENUM);
		m_Con.Color = _Value.Color;
	}



	KUINT units_size()
	{
		return (KUINT)m_UMap.size();
	}

	KPtr<Force_Unit> Find_Unit(const wchar_t* _Name);


	template<typename T>
	KPtr<T> Create_Unit(const wchar_t* _Name)
	{
		if (0 == _Name[0])
		{
			return nullptr;
		}

		KPtr<T> TU = new T(_Name, this);

		TU->Init();
		m_UMap.insert(std::map<std::wstring, KPtr<Force_Unit>>::value_type(_Name, TU));

		return TU;
	}

public:
	SC2_Force(const wchar_t* _Name, const KColor& _Color);
	~SC2_Force();
};

