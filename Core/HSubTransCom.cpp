#include "HSubTransCom.h"
#include "HTrans.h"
#include "KMacro.h"


HSubTransCom::HSubTransCom()
	: m_eMode(HSubTransCom::NONE)
	, m_Scale(1.0f), m_Rot(0.0f), m_Pivot(0.0f)
{
	m_SubWMat.Iden();
}


HSubTransCom::~HSubTransCom()
{
}

void HSubTransCom::SubTransUpdate() 
{
	// �θ��� ����� �����ָ� �Ǵ°� �ȵǴ°�?
	switch (m_eMode)
	{
	case HSubTransCom::NONE:
		m_SubWMat = m_Trans->CWMat();
		break;
	case HSubTransCom::PARENT:
		m_SMat.Scale(m_Scale); // ũ
		m_RMatX.RotX(m_Rot.x); // X
		m_RMatY.RotY(m_Rot.y); // Y
		m_RMatZ.RotZ(m_Rot.z); // Z
		m_RMat = m_RMatX * m_RMatY * m_RMatZ;
		m_PMat.Trans(m_Pivot); // ��
		m_SubWMat = m_SMat * m_RMat * m_PMat * m_Trans->CWMat();
		break;
	case HSubTransCom::SELF:
		m_SMat.Scale(m_Scale); // ũ
		m_RMatX.RotX(m_Rot.x); // X
		m_RMatY.RotY(m_Rot.y); // Y
		m_RMatZ.RotZ(m_Rot.z); // Z
		m_RMat = m_RMatX * m_RMatY * m_RMatZ;
		m_PMat.Trans(m_Pivot + m_Trans->WPos()); // ��
		m_SubWMat = m_SMat * m_RMat * m_PMat;
		break;
	default:
		break;
	}
}

void HSubTransCom::ComInit()
{
	HBaseCom::ComInit();

	KASSERT(nullptr == m_Trans);

	if (nullptr == m_Trans)
	{
		return;
	}
}