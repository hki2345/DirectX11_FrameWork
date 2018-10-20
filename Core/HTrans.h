#pragma once
#include "KMath.h"
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

	KVector4 m_LScale;		// 크
	KVector4 m_LRot;		// 자
	KVector4 m_LPos;		// 이

	KVector4 m_WRot;		// 이
	KVector4 m_WScale;		// 크
	KVector4 m_WPos;		// 이

	KVector4 m_LRevolve;	// 공

	// 기저벡터
	KVector4 m_LAx[AXIS::AX_MAX];		// 이
	KVector4 m_WAx[AXIS::AX_MAX];		// 이

	KMatrix m_SMat; // 크
	KMatrix m_RMat; // 자
	KMatrix m_PMat; // 이

	KMatrix m_WMat; // 이

	KMatrix m_RMatX, m_RMatY, m_RMatZ; // 자



public:
	bool IsWorldUpdate() {		return m_bW;	}


	void LPos(const KVector4& _Value)
	{
		m_LPos = _Value;

		if (nullptr != Actor()->Parent())
		{
			m_WPos = Actor()->Trans()->WPos() + m_LPos;
		}

		m_bLP = true;
		m_bW = true;
	}


	void LMove(const KVector4& _Value)
	{
		m_LPos += _Value;
		m_bLP = true;
		m_bW = true;
	}

	void WPos(const KVector4& _Value)
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

	void LAccRotDeg(const KVector4& _Value)
	{
		m_LRot += _Value * HMATHVAR::DTOR;
		m_bLR = true;
		m_bW = true;
	}

	void LAccRotRad(const KVector4& _Value)
	{
		m_LRot += _Value;
		m_bLR = true;
		m_bW = true;
	}

	void LRotRad(const KVector4& _Value)
	{
		m_LRot = _Value;
		m_bLR = true;
		m_bW = true;
	}

	void LRot(const KVector4& _Value)
	{
		m_LRot = _Value * HMATHVAR::DTOR;
		m_bLR = true;
		m_bW = true;
	}

	void WRot(const KVector4& _Value) 
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


	void LScale(const KVector4& _Scale)
	{
		// 여기서 바로 계산해 버리는 방법이 있고.
		m_LScale = _Scale;
		m_bLS = true;
		m_bW = true;
	}

	void WScale(const KVector4& _Scale)
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

	KVector4 LForward() {		return m_LAx[AX_F];	}
	KVector4 WForward() {		return m_WAx[AX_F];	}
	KVector4 LUp() { return m_LAx[AX_U]; }
	KVector4 WUp() { return m_WAx[AX_U]; }
	KVector4 LRight() { return m_LAx[AX_R]; }
	KVector4 WRight() { return m_WAx[AX_R]; }
	KVector4 LBack() { return -m_LAx[AX_F]; }
	KVector4 WBack() { return -m_WAx[AX_F]; }
	KVector4 LLeft() { return -m_LAx[AX_R]; }
	KVector4 WLeft() { return -m_WAx[AX_R]; }
	KVector4 LDown() { return -m_LAx[AX_U]; }
	KVector4 WDown() { return -m_WAx[AX_U]; }

	KVector4 LPos() const	{ return m_LPos; }
	KVector4 WPos() const { return m_WPos; }
	KVector4 LScale() const { return m_LScale; }

	KVector4 WScale() const { return m_WScale; }
	const KVector4& CWScale() const { return m_WScale; }

	KVector4 LRot()	{ return m_LRot;	}
	KVector4 WRot() { return m_WRot; }

	KMatrix WMat() const
	{
		return m_WMat;
	}

	
		
		

	const KMatrix& CPMat() const
	{
		return m_PMat;
	}

	const KMatrix& CSMat() const
	{
		return m_SMat;
	}

	const KMatrix& CRMat() const
	{
		return m_RMat;
	}

	const KMatrix& CWMat() const
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

