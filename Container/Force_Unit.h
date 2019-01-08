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

enum PLAYABLE_TYPE
{
	PBT_NONE = 0,
	PBT_ENEMY,
	PBT_USER,
};

// 유닛이름은 -> 유닛 정보를 불러오고
// 유닛 정보 내에 있는 이름은 해당 메쉬를 불러온다.
class SC2_Force;
class Renderer_BonAni;
class KBox_Col;
class KCollision;
class Force_Unit : public Component
{
public:
	enum ANI_TYPE
	{
		ALL = -1,
		STAND01 = 0,
		STAND02,
		STAND03,
		WALK01,
		WALK02,
		ATTACK01,
		ATTACK02,
		ATTACK03,
		FIDGET01,
		FIDGET02,
		DEATH
	};


	class Unit_Info
	{
	public:
		// 핵심 네 개의 정보
		WEAPON_TYPE		WType;
		float			LSpeed;
		float			RSpeed;
		KVector4		UScale;
		PLAYABLE_TYPE	PBType;
	};


	// 유닛의 속성은 누적되는 방식이기 때문에 이 방식으로 하는 게 맞다.
	// 이 방식 - 비트단위 연산
private:
	KVector								m_ColPivot;

	KPtr<KBox_Col>						m_Col;
	KPtr<SC2_Force> 					m_Force;
	Unit_Info							m_Info;
	ANI_TYPE							m_AType;

	std::list<std::wstring>				m_StrList;
	std::list<std::wstring>::iterator	m_SCI;
	std::list<std::wstring>::iterator	m_ECI;

	std::list<KPtr<Renderer_BonAni>>			m_RList;
	std::list<KPtr<Renderer_BonAni>>::iterator	m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator	m_ERI;

private:
	void Update_StrList();
	void Update_RenderAni();
	void Update_Col();

	void Collision_OtherUnit(KCollision* _Left, KCollision* _Right);


public:
	virtual bool Init(const wchar_t* _Name);
	virtual void Update();


	bool Load(const wchar_t* _Name);
	bool Save();

	void Reset_Renderer();
	void Delete_Renderer(KPtr<Renderer_BonAni> _Other);
	void Insert_Renderer(KPtr<Renderer_BonAni> _Other);
	void Insert_Collider();

	void Rot_Unit(const KVector& _Value)
	{
		m_ColPivot = _Value;
	}

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


#pragma region UNITINFO
	void weapon_type(const WEAPON_TYPE& _Value)
	{
		m_Info.WType = _Value;
	}
	WEAPON_TYPE& weapon_type()
	{
		return m_Info.WType;
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

	void scale_unit(const KVector& _Value)
	{
		m_Info.UScale = _Value;
	}
	KVector& scale_unit()
	{
		return m_Info.UScale;
	}

	void playable_type(const PLAYABLE_TYPE& _Value)
	{
		m_Info.PBType = _Value;
	}
	PLAYABLE_TYPE& playable_type()
	{
		return m_Info.PBType;
	}
#pragma endregion

	void Set_Animation(const ANI_TYPE& _Value)
	{
		m_AType = _Value;
	}

public:
	Force_Unit();
	~Force_Unit();
};

