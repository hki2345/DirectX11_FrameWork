#include "TransPosition.h"
#include "TheOne.h"


bool TransPosition::IsMulti(TheOne* _pOne)
{
	// 이놈이 있다면 그놈을 반환한다. => 없어야 추가를 하는 형태
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
	// 정보가 바뀔때만 실행한다.
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

			// 메모리 생각 안하고 일단 함
			KMatrix WRotMat;
			KMatrix TempWRotMatX;
			KMatrix TempWRotMatY;
			KMatrix TempWRotMatZ;

			TempWRotMatX.Rotate_X(m_WorldRotate.x); // X
			TempWRotMatY.Rotate_Y(m_WorldRotate.y); // Y
			TempWRotMatZ.Rotate_Z(m_WorldRotate.z); // Z
										 // 자전
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

	// 부모가 있는지 체크
	if (nullptr == one()->parent())
	{
		// 부모가 없으면 그냥 크자이
		if (true == m_bWorld)
		{
			m_Mat_World = m_Mat_Scale * m_Mat_Rotate * m_Mat_Position;

			m_WorldPosition = m_Mat_World.vec4;
			m_WorldScale = KVector(m_Mat_World.vec1.x, m_Mat_World.vec2.y, m_Mat_World.vec3.z);
		}
	}
	else if (nullptr != one()->parent()->transform())
	{
		// 부모가 있으면 크자이 * 부모 이동
		if (true == m_bWorld || true == one()->parent()->transform()->m_bWorld)
		{
			// 렌더스프라이트에서 -> 트랜스폼이 변한 것을 알아야한다.
			// 비단 렌더문제만이 아니라... 부모가 변했다고 자식이 변하지 않았으므로...;;
			// 그걸 알 수있는 시점이 여기고 그걸 받아서 쓰는 컴퍼넌트들이 변화한 것을 감지한다.
			m_bWorld = true;
			m_Mat_World = m_Mat_Scale * m_Mat_Rotate * m_Mat_Position * 
				one()->parent()->transform()->World_Matrix();

			// 위치와 크기는 추줄 가능 - 회전 추출 불가
			m_WorldPosition = m_Mat_World.vec4;
			m_WorldScale = KVector(m_Mat_World.vec1.x, m_Mat_World.vec2.y, m_Mat_World.vec3.z);
		}
	}
}

void TransPosition::End_Update()
{
	m_bWorld = false;
}
