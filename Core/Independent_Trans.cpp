#include "Independent_Trans.h"
#include "TransPosition.h"
#include "KMacro.h"

Independent_Trans::Independent_Trans()
	: m_eMode(INDEPEN_MODE::BASE),
	m_Scale(1.0f), m_Rotate(.0f), m_Pivot(.0f)
{
	// �ϴ� �⺻���� �׵���� -> �θ�� ���� ���� �����ϴ� ��Ȳ
	m_IndenMat.Identity();
}


Independent_Trans::~Independent_Trans()
{
}

void Independent_Trans::Indepen_Update()
{
	// ���ִ°� ������?
	if (false == m_Trans->BWorld())
	{
		return;
	}

	switch (m_eMode)
	{
		// ���� ����� �ƹ� ���� ���� �� �ڱⰡ �Ǵ� ��Ȳ
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
		// ������ü�� 
	case Independent_Trans::IDEPENDENT:
	{
		m_ScaleMat.Scale(m_Scale);
		m_RMatX.Rotate_X(m_Rotate.x);
		m_RMatY.Rotate_Y(m_Rotate.y);
		m_RMatZ.Rotate_Z(m_Rotate.z);
		m_RotateMat = m_RMatX * m_RMatY * m_RMatZ;

		// �θ��� ��ġ������ �����ǰ�
		m_PosMat.Translation(m_Pivot + m_Trans->World_Pos());

		//������� - ������ ���� �ʴ´�.
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