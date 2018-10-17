#pragma once
#include "GameMath.h"
#include "HComponent.h"

// ���� ���� �̷��� �����Ѵ�.
// ��� �� ������Ʈ�� ���� ����� �̾Ƴ��� �Ѵ�.
// ���� ����� �̾Ƴ�����
// ũ���̰���

// ũ���̰��θ� �̾Ƴ����� ���� ���� ���� �־�� �ϴ°�?
// ũ��
// ȸ��
// ��ġ
// ��ġ ȸ��
// �θ�

// ǥ���ϱ� ����
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

	HVEC m_LScale;		// ũ
	HVEC m_LRot;		// ��
	HVEC m_LPos;		// ��

	HVEC m_WRot;		// ��
	HVEC m_WScale;		// ũ
	HVEC m_WPos;		// ��

	HVEC m_LRevolve;	// ��

	// ��������
	HVEC m_LAx[AXIS::AX_MAX];		// ��
	HVEC m_WAx[AXIS::AX_MAX];		// ��

	HMAT m_SMat; // ũ
	HMAT m_RMat; // ��
	HMAT m_PMat; // ��

	HMAT m_WMat; // ��

	HMAT m_RMatX, m_RMatY, m_RMatZ; // ��



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
		// ���⼭ �ٷ� ����� ������ ����� �ְ�.
		// �θ� �����ϰ� ���� ������ �ű�� ���� �ȴ�.
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
		// ���⼭ �ٷ� ����� ������ ����� �ְ�.
		m_LScale = _Scale;
		m_bLS = true;
		m_bW = true;
	}

	void WScale(const HVEC& _Scale)
	{
		// ���⼭ �ٷ� ����� ������ ����� �ְ�.
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

	// ���

public:
	HTrans();
	~HTrans();
};

