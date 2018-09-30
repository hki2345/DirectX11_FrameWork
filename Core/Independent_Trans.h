#pragma once
#include "Component_DE.h"

// 부모에 종속되어있어도 독립적으로 행동할 수 있게끔 해줌
class Independent_Trans : public Component_DE
{
private:
	enum INDEPEN_MODE
	{
		BASE,
		CONTROLLED,
		IDEPENDENT,
	};

private:
	INDEPEN_MODE m_eMode;
	KVector4 m_Scale;
	KVector4 m_Rotate;

	// 위치는 무조건 부모를 따르게 되어있음
	KVector4 m_Pivot;

	KMatrix m_ScaleMat;
	KMatrix m_RotateMat;
	KMatrix m_PosMat;

	KMatrix m_RMatX, m_RMatY, m_RMatZ; // 공전이 아닌 자전
	KMatrix m_IndenMat; // 독립적인 메트릭스 -> 이것을 산출한다.

public:
	void Init_Component() override;
	
	
	void Base_Mode() { m_eMode = INDEPEN_MODE::BASE; }
	void Controlled_Mode()  { m_eMode = INDEPEN_MODE::CONTROLLED; }
	void Independent_Mode()  { m_eMode = INDEPEN_MODE::IDEPENDENT; }

	void inde_scale(const KVector4& _Value)
	{
		m_Scale = _Value;
		m_Trans->BLocalScale(true);
		m_Trans->BWorld(true);
		Independent_Mode();
	}
	void inde_rotation(const KVector4& _Value)
	{
		m_Rotate = _Value;
		m_Trans->BLocalRotate(true);
		m_Trans->BWorld(true);
		Independent_Mode();
	}
	void inde_pivot(const KVector4& _Value) 
	{
		m_Pivot = _Value;
		m_Trans->BLocalPostion(true);
		m_Trans->BWorld(true);
		Independent_Mode();
	}

	KVector4& inde_scale()
	{
		return m_Scale;
	}
	KVector4& inde_rotation()
	{
		return m_Rotate;
	}
	KVector4& inde_pivot()
	{
		return m_Pivot;
	}

	KVector4 pivot_world() { return m_IndenMat.vec4; }

	const KMatrix& independent_Matrix() const { return m_IndenMat; }
	void Indepen_Update();

public:
	Independent_Trans();
	~Independent_Trans();
};

