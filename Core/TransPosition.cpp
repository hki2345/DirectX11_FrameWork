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
	// ������ �ٲ𶧸� �����Ѵ�.
	if (true == m_bLocalScale)
	{
		m_Mat_Scale.Scale(m_LocalScale);
		m_bLocalScale = false;
	}
	
	if (true == m_bLocalRotate)
	{
		m_Mat_RotX.Rotate_X(m_LocalRotate.x);
		m_Mat_RotY.Rotate_Y(m_LocalRotate.y);
		m_Mat_RotZ.Rotate_Z(m_LocalRotate.z);

		m_Mat_Rotate = m_Mat_RotX * m_Mat_RotY * m_Mat_RotZ;

		m_LocalAx[Axis::Ax_Forward] = KVector4::Forword;
		m_LocalAx[Axis::Ax_Up] = KVector4::Up;
		m_LocalAx[Axis::Ax_Right] = KVector4::Right;

		for (size_t i = 0; i < Axis::Ax_Num; i++)
		{
			m_LocalAx[i] = m_Mat_Rotate.Multi_Vector_Z(m_LocalAx[i]);
			m_LocalAx[i].NormalizeVec3();
		}

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


	if (true == m_bLocalPostion)
	{
		m_Mat_Position.Translation(m_LocalPosition);
		m_bLocalPostion = false;
	}

	// �θ� �ִ��� üũ
	if (nullptr == one()->parent())
	{
		// �θ� ������ �׳� ũ����
		if (true == m_bWorld)
		{
			m_Mat_World = m_Mat_Scale * m_Mat_Rotate * m_Mat_Position;

			m_WorldPosition = m_Mat_World.vec4;
			m_WorldScale = KVector(m_Mat_World.vec1.x, m_Mat_World.vec2.y, m_Mat_World.vec3.z);
		}
	}
	else if (nullptr != one()->parent()->transform())
	{
		// �θ� ������ ũ���� * �θ� �̵�
		if (true == m_bWorld || true == one()->parent()->transform()->m_bWorld)
		{
			// ������������Ʈ���� -> Ʈ�������� ���� ���� �˾ƾ��Ѵ�.
			// ��� ������������ �ƴ϶�... �θ� ���ߴٰ� �ڽ��� ������ �ʾ����Ƿ�...;;
			// �װ� �� ���ִ� ������ ����� �װ� �޾Ƽ� ���� ���۳�Ʈ���� ��ȭ�� ���� �����Ѵ�.
			m_bWorld = true;
			m_Mat_World = m_Mat_Scale * m_Mat_Rotate * m_Mat_Position * 
				one()->parent()->transform()->World_Matrix();

			// ��ġ�� ũ��� ���� ���� - ȸ�� ���� �Ұ�
			m_WorldPosition = m_Mat_World.vec4;
			m_WorldScale = KVector(m_Mat_World.vec1.x, m_Mat_World.vec2.y, m_Mat_World.vec3.z);
		}
	}
}

void TransPosition::End_Update()
{
	m_bWorld = false;
}
