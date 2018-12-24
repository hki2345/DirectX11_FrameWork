#pragma once
#include <Component.h>
#include <list>


#include "Container.h"

enum WEAPON_TYPE
{
	NONE  = 0,
	INSTANCE,
	SHOT,
};

// 유닛이름은 -> 유닛 정보를 불러오고
// 유닛 정보 내에 있는 이름은 해당 메쉬를 불러온다.
class SC2_Force;
class Renderer_BonAni;
class KSphere_Col;
class Force_Unit : public Component
{
public:
	class Unit_Info
	{
	public:
		// 핵심 네 개의 정보
		WEAPON_TYPE		WTpye;
		float			LSpeed;
		float			RSpeed;
		KVector4		UScale;
	};


	// 유닛의 속성은 누적되는 방식이기 때문에 이 방식으로 하는 게 맞다.
	// 이 방식 - 비트단위 연산
private:
	KPtr<KSphere_Col>					m_Col;
	KPtr<SC2_Force> 					m_Force;
	Unit_Info							m_Info;


	std::list<std::wstring>				m_StrList;
	std::list<std::wstring>::iterator	m_SCI;
	std::list<std::wstring>::iterator	m_ECI;

	std::list<KPtr<Renderer_BonAni>>			m_RList;
	std::list<KPtr<Renderer_BonAni>>::iterator	m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator	m_ERI;

private:
	void Update_StrList();

public:
	virtual bool Init(const wchar_t* _Name);
	virtual void Update();


	bool Load(const wchar_t* _Name);
	bool Save();

	void Reset_Renderer();
	void Delete_Renderer(KPtr<Renderer_BonAni> _Other);
	void Insert_Renderer(KPtr<Renderer_BonAni> _Other);



	void force(KPtr<SC2_Force> _Force)
	{
		m_Force = _Force;
	}

	Unit_Info& unit_info()
	{
		return m_Info;
	}
	void unit_info(const Unit_Info& _Value)
	{
		m_Info = _Value;
	}


	std::list<std::wstring>* list_string()
	{
		return &m_StrList;
	}
	std::list<KPtr<Renderer_BonAni>> list_renderer()
	{
		return m_RList;
	}

	void weapon_type(const WEAPON_TYPE& _Value)
	{
		m_Info.WTpye = _Value;
	}
	WEAPON_TYPE& weapon_type()
	{
		return m_Info.WTpye;
	}

	float& linear_speed()
	{
		return m_Info.LSpeed;
	}
	float& rotate_speed()
	{
		return m_Info.RSpeed;
	}

	void linear_speed(const float& _Value)
	{
		m_Info.LSpeed = _Value;
	}
	void rotate_speed(const float& _Value)
	{
		m_Info.RSpeed = _Value;
	}

	KVector& unit_scale()
	{
		return m_Info.UScale;
	}
	void unit_scale(const KVector& _Value)
	{
		m_Info.UScale = _Value;
	}


public:
	Force_Unit();
	~Force_Unit();
};

