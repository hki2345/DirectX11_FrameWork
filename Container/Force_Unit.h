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

// �����̸��� -> ���� ������ �ҷ�����
// ���� ���� ���� �ִ� �̸��� �ش� �޽��� �ҷ��´�.
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
		// �ٽ� �� ���� ����
		WEAPON_TYPE		WTpye;
		float			LSpeed;
		float			RSpeed;
		KVector4		UScale;
	};


	// ������ �Ӽ��� �����Ǵ� ����̱� ������ �� ������� �ϴ� �� �´�.
	// �� ��� - ��Ʈ���� ����
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

	void unit_scale(const KVector& _Value)
	{
		m_Info.UScale = _Value;
	}
	KVector& unit_scale()
	{
		return m_Info.UScale;
	}

	void Set_Animation(const ANI_TYPE& _Value)
	{
		m_AType = _Value;
	}

public:
	Force_Unit();
	~Force_Unit();
};

