#pragma once
#include "Force_Unit.h"


class Camera;
class Renderer_BonAni;
class KSphere_Col;
class Unit_Marine :	public Force_Unit
{
	enum SKILL_ULTI
	{
		ULTI_TANK,
		ULTI_THOR,
		ULTI_BATTLE,
	};


private:
	// 시한 부 인생들
	// 탱크 8대 토르 5대 배틀 7대
	SKILL_ULTI m_Ulti;

	// Left - Right
	std::vector<float> G_IntVec;
	std::vector<float> GCur_IntVec;

	// Shift - E - Q
	// 스팀팩 - 회복기 및 귀환 - 궁극기 말그대로
	// 달리기 - 의료선 - 궁극기
	std::vector<float> SK_IntVec;
	std::vector<float> SKCur_IntVec;

	float m_Speed;
	float m_Boost;



	KPtr<Camera>			m_PCam;
	KPtr<Renderer_BonAni>	m_Ren;
	KPtr<KSphere_Col>		m_Col;



public:
	bool Init();
	void Update();

public:
	Unit_Marine();
	~Unit_Marine();
};

