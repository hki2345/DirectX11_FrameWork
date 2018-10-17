#include "HTrans.h"
#include "HActor.h"
bool HTrans::IsMulti(HActor* _pActor) {

	if (nullptr != _pActor->GetCom<HTrans>())
	{
		return false;
	}

	return true;
}

HTrans::HTrans() : m_LScale(1.0f), m_bLS(true), m_bLR(true), m_bLP(true), m_bW(true)
{
	m_LAx[AXIS::AX_F] = HVEC::FORWORD;
	m_LAx[AXIS::AX_U] = HVEC::UP;
	m_LAx[AXIS::AX_R] = HVEC::RIGHT;

	m_WAx[AXIS::AX_F] = HVEC::FORWORD;
	m_WAx[AXIS::AX_U] = HVEC::UP;
	m_WAx[AXIS::AX_R] = HVEC::RIGHT;
}


HTrans::~HTrans()
{
}

void HTrans::Reset()
{
	m_LPos = 0.0f;
	m_LRot = 0.0f;
	m_LScale = 1.0f;
	m_bLS, m_bLR, m_bLP, m_bW = true;

	m_LAx[AXIS::AX_F] = HVEC::FORWORD;
	m_LAx[AXIS::AX_U] = HVEC::UP;
	m_LAx[AXIS::AX_R] = HVEC::RIGHT;

	m_WAx[AXIS::AX_F] = HVEC::FORWORD;
	m_WAx[AXIS::AX_U] = HVEC::UP;
	m_WAx[AXIS::AX_R] = HVEC::RIGHT;
}

bool HTrans::Init()
{
	Actor()->Trans(this);
	return true;
}

void HTrans::FinalUpdate()
{
	if (true == m_bLS)
	{
		m_SMat.Scale(m_LScale); // 크
		m_bLS = false;
	}

	if (true == m_bLR)
	{
		m_RMatX.RotX(m_LRot.x); // X
		m_RMatY.RotY(m_LRot.y); // Y
		m_RMatZ.RotZ(m_LRot.z); // Z
							   // 자전
		m_RMat = m_RMatX * m_RMatY * m_RMatZ;

		m_LAx[AXIS::AX_F] = HVEC::FORWORD;
		m_LAx[AXIS::AX_U] = HVEC::UP;
		m_LAx[AXIS::AX_R] = HVEC::RIGHT;
		for (size_t i = 0; i < AXIS::AX_MAX; i++)
		{
			m_LAx[i] = m_RMat.MulVecZero(m_LAx[i]);
			m_LAx[i].Normalize3D();
		}

		if (nullptr != Actor()->Parent())
		{
			m_WRot = Actor()->Trans()->WRot() + m_LRot;

			HMAT WRotMat;
			HMAT TempWRotMatX;
			HMAT TempWRotMatY;
			HMAT TempWRotMatZ;

			TempWRotMatX.RotX(m_WRot.x); // X
			TempWRotMatY.RotY(m_WRot.y); // Y
			TempWRotMatZ.RotZ(m_WRot.z); // Z
										 // 자전
			WRotMat = TempWRotMatX * TempWRotMatY * TempWRotMatZ;

			m_WAx[AXIS::AX_F] = HVEC::FORWORD;
			m_WAx[AXIS::AX_U] = HVEC::UP;
			m_WAx[AXIS::AX_R] = HVEC::RIGHT;

			for (size_t i = 0; i < AXIS::AX_MAX; i++)
			{
				m_WAx[i] = WRotMat.MulVecZero(m_WAx[i]);
				m_WAx[i].Normalize3D();
			}
		}
		else {
			for (size_t i = 0; i < AXIS::AX_MAX; i++)
			{
				m_WAx[i] = m_LAx[i];
				m_WAx[i].Normalize3D();
			}
		}

		m_bLR = false;
	}

	if (true == m_bLP)
	{
		m_PMat.Trans(m_LPos); // 이
		m_bLP = false;
	}

	if (nullptr == Actor()->Parent())
	{
		if (true == m_bW)
		{
			m_WMat = m_SMat * m_RMat * m_PMat;

			m_WPos = m_WMat.v4;
			m_WScale = HVEC(m_WMat.v1.x, m_WMat.v2.y, m_WMat.v3.z);
		}
	}
	else if (nullptr != Actor()->Parent()->Trans())
	{
		if (true == m_bW || true == Actor()->Parent()->Trans()->m_bW)
		{
			HMAT WW = Actor()->Parent()->Trans()->WMat();
			m_WMat = m_SMat * m_RMat * m_PMat * Actor()->Parent()->Trans()->WMat();

			m_WPos = m_WMat.v4;
			m_WScale = HVEC(m_WMat.v1.x, m_WMat.v2.y, m_WMat.v3.z);
		}
	}
}

void HTrans::EndUpdate() 
{
	m_bW = false;
}