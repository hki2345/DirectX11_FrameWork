#include "TransPosition.h"
#include "TheOne.h"


bool TransPosition::IsMulti(TheOne* _pOne)
{
	// �̳��� �ִٸ� �׳��� ��ȯ�Ѵ�. => ����� �߰��� �ϴ� ����
	if (nullptr != _pOne->Get_Component<TransPosition>())
	{
		return false;
	}
	return true;
}

TransPosition::TransPosition() : m_LocalScale(1.0f),
m_bWorld(true),
m_bLocalScale(true),
m_bLocalRotate(true),
m_bLocalPostion(true)
{
	m_LocalAx[Axis::Ax_Forward]	= KVector4::Forword;
	m_LocalAx[Axis::Ax_Up]		= KVector4::Up;
	m_LocalAx[Axis::Ax_Right]	= KVector4::Right;

	m_WorldAx[Axis::Ax_Forward]	= KVector4::Forword;
	m_WorldAx[Axis::Ax_Up]		= KVector4::Up;
	m_WorldAx[Axis::Ax_Right]	= KVector4::Right;
}


TransPosition::~TransPosition()
{
}


void TransPosition::reset()
{
	m_LocalScale = 1.0f;
	m_LocalRotate = .0f;
	m_LocalPosition = .0f;
	m_bLocalPostion, m_bLocalRotate, m_bLocalScale, m_bWorld = true;


	m_LocalAx[Axis::Ax_Forward] = KVector4::Forword;
	m_LocalAx[Axis::Ax_Up] = KVector4::Up;
	m_LocalAx[Axis::Ax_Right] = KVector4::Right;

	m_WorldAx[Axis::Ax_Forward] = KVector4::Forword;
	m_WorldAx[Axis::Ax_Up] = KVector4::Up;
	m_WorldAx[Axis::Ax_Right] = KVector4::Right;
}

bool TransPosition::Init()
{
	one()->transform(this);
	return true;
}

void TransPosition::FinalUpdate()
{
	// ������ �ٲ𶧸� �����Ѵ�. - ������
	if (true == m_bLocalScale)
	{
		m_ScaleMat.Scale(m_LocalScale);
		m_bLocalScale = false;
	}
	
	// ȸ�� üũ
	if (true == m_bLocalRotate)
	{
		m_RotXMat.Rotate_X(m_LocalRotate.x);
		m_RotYMat.Rotate_Y(m_LocalRotate.y);
		m_RotZMat.Rotate_Z(m_LocalRotate.z);

		m_RotMat = m_RotXMat * m_RotYMat * m_RotZMat;

		m_LocalAx[Axis::Ax_Forward] = KVector4::Forword;
		m_LocalAx[Axis::Ax_Up] = KVector4::Up;
		m_LocalAx[Axis::Ax_Right] = KVector4::Right;

		for (size_t i = 0; i < Axis::Ax_Num; i++)
		{
			m_LocalAx[i] = m_RotMat.Multi_Vector_Z(m_LocalAx[i]);
			m_LocalAx[i].NormalizeVec3();
		}

		// �θ� ������
		if (nullptr != one()->parent())
		{
			m_WorldRotate = one()->transform()->World_Rotate() + m_LocalRotate;

			// �޸� ���� ���ϰ� �ϴ� ��
			KMatrix WRotMat;
			KMatrix TempWRotMatX;
			KMatrix TempWRotMatY;
			KMatrix TempWRotMatZ;

			TempWRotMatX.Rotate_X(m_WorldRotate.x); // X
			TempWRotMatY.Rotate_Y(m_WorldRotate.y); // Y
			TempWRotMatZ.Rotate_Z(m_WorldRotate.z); // Z
										 // ����
			WRotMat = TempWRotMatX * TempWRotMatY * TempWRotMatZ;

			m_WorldAx[Axis::Ax_Forward] = KVector::Forword;
			m_WorldAx[Axis::Ax_Up] = KVector::Up;
			m_WorldAx[Axis::Ax_Right] = KVector::Right;

			for (size_t i = 0; i < Axis::Ax_Num; i++)
			{
				m_WorldAx[i] = WRotMat.Multi_Vector_Z(m_WorldAx[i]);
				m_WorldAx[i].NormalizeVec3();
			}
		}
		else {
			for (size_t i = 0; i < Axis::Ax_Num; i++)
			{
				m_WorldAx[i] = m_LocalAx[i];
				m_WorldAx[i].NormalizeVec3();
			}
		}

		m_bLocalRotate = false;
	}

	// ��ġ üũ
	if (true == m_bLocalPostion)
	{
		m_PosMat.Translation(m_LocalPosition);
		m_bLocalPostion = false;
	}



	// ���� ����
	// �θ� �ִ��� üũ
	if (nullptr == one()->parent())
	{
		// �θ� ������ �׳� ũ����
		if (true == m_bWorld)
		{
			m_WorldMat = m_ScaleMat * m_RotMat * m_PosMat;

			m_WorldPosition = m_WorldMat.vec4;
			m_WorldScale = KVector(m_WorldMat.vec1.x, m_WorldMat.vec2.y, m_WorldMat.vec3.z);
		}
	}
	else if (nullptr != one()->parent()->transform())
	{
		// �θ� ������ ũ���� * �θ� �̵� - �ѹ� �� �̵�
		if (true == m_bWorld || true == one()->parent()->transform()->m_bWorld)
		{
			// ������������Ʈ���� -> Ʈ�������� ���� ���� �˾ƾ��Ѵ�.
			// ��� ������������ �ƴ϶�... �θ� ���ߴٰ� �ڽ��� ������ �ʾ����Ƿ�...;;
			// �װ� �� ���ִ� ������ ����� �װ� �޾Ƽ� ���� ���۳�Ʈ���� ��ȭ�� ���� �����Ѵ�.
			m_bWorld = true;
			m_WorldMat = m_ScaleMat * m_RotMat * m_PosMat * 
				one()->parent()->transform()->World_Matrix();

			// ��ġ�� ũ��� ���� ���� - ȸ�� ���� �Ұ�
			m_WorldPosition = m_WorldMat.vec4;
			m_WorldScale = KVector(m_WorldMat.vec1.x, m_WorldMat.vec2.y, m_WorldMat.vec3.z);
		}
	}
}

void TransPosition::End_Update()
{
	m_bWorld = false;
}