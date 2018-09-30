#include "Independent_Trans.h"
#include "TransPosition.h"
#include "KMacro.h"

Independent_Trans::Independent_Trans()
	: m_eMode(INDEPEN_MODE::BASE),
	m_Scale(1.0f), m_Rotate(.0f), m_Pivot(.0f)
{
	// 일단 기본적인 항등행렬 -> 부모와 완전 따로 생각하는 상황
	m_IndenMat.Identity();
}


Independent_Trans::~Independent_Trans()
{
}

void Independent_Trans::Indepen_Update()
{
	// 해주는게 좋겟지?
	if (false == m_Trans->BWorld())
	{
		return;
	}

	switch (m_eMode)
	{
		// 월드 행렬이 아무 연산 없이 곧 자기가 되는 상황
	case Independent_Trans::BASE:
		m_IndenMat = m_Trans->World_Matrix_Const();
		break;
	case Independent_Trans::CONTROLLED:
	{
		KVector4 Temp_Scale = m_Trans->Local_Scale();
		KVector4 Temp_Rotate = m_Trans->Local_Rotate();

		m_Rotate /= Temp_Rotate;

		m_ScaleMat.Scale(m_Scale / Temp_Scale);
		m_RMatX.Rotate_X(m_Rotate.x);
		m_RMatY.Rotate_Y(m_Rotate.y); 
		m_RMatZ.Rotate_Z(m_Rotate.z); 
		m_RotateMat = m_RMatX * m_RMatY * m_RMatZ;
		m_PosMat.Translation(m_Pivot / Temp_Scale);

		m_IndenMat = m_ScaleMat * m_RotateMat * m_PosMat * m_Trans->World_Matrix_Const();
		break;
	}
		// 독립객체면 
	case Independent_Trans::IDEPENDENT:
	{
		m_ScaleMat.Scale(m_Scale);
		m_RMatX.Rotate_X(m_Rotate.x);
		m_RMatY.Rotate_Y(m_Rotate.y);
		m_RMatZ.Rotate_Z(m_Rotate.z);
		m_RotateMat = m_RMatX * m_RMatY * m_RMatZ;

		// 부모의 위치까지만 관여되고
		m_PosMat.Translation(m_Pivot + m_Trans->World_Pos());

		//월드행렬 - 공전은 되지 않는다.
		m_IndenMat = m_ScaleMat * m_RotateMat * m_PosMat;
		break;
	}

	default:
		break;
	}
}

void Independent_Trans::Init_Component()
{
	Component_DE::Init_Component();

	KASSERT(m_Trans == nullptr);
	if (nullptr == m_Trans)
	{
		return;
	}
}