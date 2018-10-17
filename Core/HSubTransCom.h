#pragma once
#include "HBaseCom.h"

// 충돌체
// 랜더러 등이 사용할 것이다.
class HSubTransCom : public HBaseCom
{
private:
	enum SUBMODE
	{
		NONE,
		PARENT,
		SELF,
	};

private:
	SUBMODE m_eMode;
	HVEC m_Scale;
	HVEC m_Rot; // 자전만 영향 준다.
	HVEC m_Pivot;

	HMAT m_SMat; // 크
	HMAT m_RMat; // 자
	HMAT m_PMat; // 이

	HMAT m_RMatX, m_RMatY, m_RMatZ; // 자

	HMAT m_SubWMat;

public:
	void SelfMode() 	{		m_eMode = SUBMODE::SELF;	}
	void NoneMode()	{		m_eMode = SUBMODE::NONE;	}
	void ParentMode() { m_eMode = SUBMODE::PARENT; }

	void SubScale(const HVEC& _Scale) {
		m_Scale = _Scale;
		SelfMode();
	}

	void SubRot(const HVEC& _Rot) {
		m_Rot = _Rot;
		SelfMode();
	}

	void SubPivot(const HVEC& _Pivot) {
		m_Pivot = _Pivot;
		SelfMode();
	}

	HVEC WSubPivot() 
	{
		return m_SubWMat.v4;
	}

public:
	const HMAT& CSWMat() const
	{
		return m_SubWMat;
	}

public:
	void ComInit() override;

public:
	void SubTransUpdate();

public:
	HSubTransCom();
	~HSubTransCom();
};

