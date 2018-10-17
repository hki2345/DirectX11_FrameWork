#pragma once
#include "GameMath.h"
#include "HComponent.h"

// 내가 지름 이렇게 존재한다.
// 어느 한 오브젝트의 월드 행렬을 뽑아내야 한다.
// 월드 행렬을 뽑아내려면
// 크자이공부

// 크자이공부를 뽑아내려면 각각 무슨 값이 있어야 하는가?
// 크기
// 회전
// 위치
// 위치 회전
// 부모

// 표현하기 위한
class HTrans final : public HComponent 
{
public:
	enum AXIS
	{
		AX_F,
		AX_U,
		AX_R,
		AX_MAX
	};

public:
	static bool IsMulti(HActor* _pActor);

private:
	bool m_bLS;
	bool m_bLR;
	bool m_bLP;
	bool m_bW;

	HVEC m_LScale;		// 크
	HVEC m_LRot;		// 자
	HVEC m_LPos;		// 이

	HVEC m_WRot;		// 이
	HVEC m_WScale;		// 크
	HVEC m_WPos;		// 이

	HVEC m_LRevolve;	// 공

	// 기저벡터
	HVEC m_LAx[AXIS::AX_MAX];		// 이
	HVEC m_WAx[AXIS::AX_MAX];		// 이

	HMAT m_SMat; // 크
	HMAT m_RMat; // 자
	HMAT m_PMat; // 이

	HMAT m_WMat; // 이

	HMAT m_RMatX, m_RMatY, m_RMatZ; // 자



public:
	bool IsWorldUpdate() {		return m_bW;	}


	void LPos(const HVEC& _Value)
	{
		m_LPos = _Value;

		if (nullptr != Actor()->Parent())
		{
			m_WPos = Actor()->Trans()->WPos() + m_LPos;
		}

		m_bLP = true;
		m_bW = true;
	}


	void LMove(const HVEC& _Value)
	{
		m_LPos += _Value;
		m_bLP = true;
		m_bW = true;
	}

	void WPos(const HVEC& _Value)
	{
		// 여기서 바로 계산해 버리는 방법이 있고.
		// 부모를 무시하고 나는 무조건 거기로 가면 된다.
		if (nullptr != Actor()->Parent())
		{
			m_LPos = _Value - Actor()->Trans()->WPos();
		}
		else {
			m_LPos = _Value;
		}
		m_WPos = _Value;
		m_bW = true;
	}

	void LAccRotDeg(const HVEC& _Value)
	{
		m_LRot += _Value * HMATHVAR::DTOR;
		m_bLR = true;
		m_bW = true;
	}

	void LAccRotRad(const HVEC& _Value)
	{
		m_LRot += _Value;
		m_bLR = true;
		m_bW = true;
	}

	void LRotRad(const HVEC& _Value)
	{
		m_LRot = _Value;
		m_bLR = true;
		m_bW = true;
	}

	void LRot(const HVEC& _Value)
	{
		m_LRot = _Value * HMATHVAR::DTOR;
		m_bLR = true;
		m_bW = true;
	}

	void WRot(const HVEC& _Value) 
	{ 
		if (nullptr != Actor()->Parent())
		{
			m_LRot = (m_WRot * HMATHVAR::DTOR) - Actor()->Trans()->WRot();
		}
		else {
			m_LRot = _Value * HMATHVAR::DTOR;
		}

		m_bLR = true;
		m_bW = true;
	}


	void LScale(const HVEC& _Scale)
	{
		// 여기서 바로 계산해 버리는 방법이 있고.
		m_LScale = _Scale;
		m_bLS = true;
		m_bW = true;
	}

	void WScale(const HVEC& _Scale)
	{
		// 여기서 바로 계산해 버리는 방법이 있고.
		if (nullptr != Actor()->Parent())
		{
			m_LScale = m_WScale - Actor()->Trans()->WScale();
		}
		else {
			m_LScale = _Scale;
		}

		m_bLS = true;
		m_bW = true;
	}

	HVEC LForward() {		return m_LAx[AX_F];	}
	HVEC WForward() {		return m_WAx[AX_F];	}
	HVEC LUp() { return m_LAx[AX_U]; }
	HVEC WUp() { return m_WAx[AX_U]; }
	HVEC LRight() { return m_LAx[AX_R]; }
	HVEC WRight() { return m_WAx[AX_R]; }
	HVEC LBack() { return -m_LAx[AX_F]; }
	HVEC WBack() { return -m_WAx[AX_F]; }
	HVEC LLeft() { return -m_LAx[AX_R]; }
	HVEC WLeft() { return -m_WAx[AX_R]; }
	HVEC LDown() { return -m_LAx[AX_U]; }
	HVEC WDown() { return -m_WAx[AX_U]; }

	HVEC LPos() const	{ return m_LPos; }
	HVEC WPos() const { return m_WPos; }
	HVEC LScale() const { return m_LScale; }

	HVEC WScale() const { return m_WScale; }
	const HVEC& CWScale() const { return m_WScale; }

	HVEC LRot()	{ return m_LRot;	}
	HVEC WRot() { return m_WRot; }

	HMAT WMat() const
	{
		return m_WMat;
	}

	
		
		

	const HMAT& CPMat() const
	{
		return m_PMat;
	}

	const HMAT& CSMat() const
	{
		return m_SMat;
	}

	const HMAT& CRMat() const
	{
		return m_RMat;
	}

	const HMAT& CWMat() const
	{
		return m_WMat;
	}

	void Reset();

public:
	virtual bool Init() override;
	virtual void FinalUpdate() override;
	virtual void EndUpdate() override;

	// 행렬

public:
	HTrans();
	~HTrans();
};

