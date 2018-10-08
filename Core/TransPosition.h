#pragma once
#include "KMath.h"
#include "Component.h"


// 모든 위치 속성에 대한 핵심
class TransPosition : public Component
{
public:
	enum Axis
	{
		Ax_Forward,
		Ax_Up,
		Ax_Right,
		Ax_Num,
	};

private:
	static bool IsMulti(TheOne* _pOne);

	// 트렌스 폼의 최신화는 계속이루어지지 않는다.
	// 바뀔 때만 연산한다.
	bool m_bLocalScale;
	bool m_bLocalRotate;
	bool m_bLocalPostion;
	bool m_bWorld;
	
	// 크기 자전 위치 공전
	KVector4 m_LocalScale;
	KVector4 m_LocalRotate;
	KVector4 m_LocalPosition;
	KVector4 m_LocalRevolve;

	KVector4 m_WorldScale;
	KVector4 m_WorldRotate;
	KVector4 m_WorldPosition;


	// 기저 벡터
	KVector4 m_LocalAx[Axis::Ax_Num];
	KVector4 m_WorldAx[Axis::Ax_Num];

	
	
	// 행렬 부분
	KMatrix m_ScaleMat;
	KMatrix m_RotXMat, m_RotYMat, m_RotZMat;
	KMatrix m_RotMat;
	KMatrix m_PosMat;

	KMatrix m_WorldMat;
	std::vector<KVector2>* m_VertexVec;

public:
	void reset();

	// 충돌체를 계산하기 위함
	KVector2 Local_RightTop()	
	{
		return KVector2(
			Local_Pos().x + (Local_Right() *  Local_Scale().x * .5f).x + (Local_Up() *  Local_Scale().y * .5f).x,
			Local_Pos().y + (Local_Right() *  Local_Scale().x * .5f).y + (Local_Up() *  Local_Scale().y * .5f).y) ;
	}	
	KVector2 Local_LeftTop()
	{
		return KVector2(
			Local_Pos().x + (Local_Left() *  Local_Scale().x * .5f).x + (Local_Up() *  Local_Scale().y * .5f).x,
			Local_Pos().y + (Local_Left() *  Local_Scale().x * .5f).y + (Local_Up() *  Local_Scale().y * .5f).y);
	}
	KVector2 Local_RightBottom()
	{
		return KVector2(
			Local_Pos().x + (Local_Right() *  Local_Scale().x * .5f).x + (Local_Down() *  Local_Scale().y * .5f).x,
			Local_Pos().y + (Local_Right() *  Local_Scale().x * .5f).y + (Local_Down() *  Local_Scale().y * .5f).y );
	}
	KVector2 Local_LeftBottom()
	{
		return KVector2(
			Local_Pos().x + (Local_Left() *  Local_Scale().x * .5f).x + (Local_Down() *  Local_Scale().y * .5f).x,
			Local_Pos().y + (Local_Left() *  Local_Scale().x * .5f).y + (Local_Down() *  Local_Scale().y * .5f).y);
	}


	KVector4 Local_Scale() const { return m_LocalScale; }
	void	Local_Scale(const KVector4& _Value)
	{
		m_LocalScale = _Value;
		m_bLocalScale = true;
		m_bWorld = true;
	}
	void	Local_Scale(const float& _X, const float& _Y, const float& _Z)
	{
		m_LocalScale = KVector(_X, _Y, _Z);
		m_bLocalScale = true;
		m_bWorld = true;
	}
	void	Local_Scale(const KVector2& _Vec, const float& _Z)
	{
		m_LocalScale = KVector(_Vec, _Z);
		m_bLocalScale = true;
		m_bWorld = true;
	}


	KVector4 Local_Pos() const	{ return m_LocalPosition; }
	void	Local_Pos(const KVector4& _Value)
	{
		m_LocalPosition = _Value;

		// 여기서 자식들은 1프레임 차이가 난다 -> 1 업데이트
		if (nullptr != one()->parent())
		{
			m_WorldPosition = one()->transform()->World_Pos() + m_LocalPosition;
		}

		m_bLocalPostion = true;
		m_bWorld = true;
	}
	void	Local_Pos(const float& _X, const float& _Y, const float& _Z)
	{
		m_LocalPosition = KVector(_X, _Y, _Z);
		m_bLocalPostion = true;
		m_bWorld = true;
	}
	void	Local_Pos(const KVector2& _Vec, const float& _Z)
	{
		m_LocalPosition = KVector(_Vec, _Z);
		m_bLocalPostion = true;
		m_bWorld = true;
	}


	KVector4 Local_Rotate_Degree() const { return m_LocalRotate * 180 / KPI; }
	KVector4 Local_Rotate() const { return m_LocalRotate; }
	void	Local_Rotate(const KVector4& _Value) 
	{
		m_LocalRotate = _Value * KPI / 180.0f;
		m_bLocalRotate = true;
		m_bWorld = true; 
	}
	void	Local_Rotate(const float& _X, const float& _Y, const float& _Z)
	{
		m_LocalRotate = KVector(_X, _Y, _Z) * KPI / 180.0f;
		m_bLocalRotate = true;
		m_bWorld = true;
	}
	void	Local_Rotate(const KVector2& _Vec, const float& _Z)
	{
		m_LocalRotate = KVector(_Vec, _Z);
		m_bLocalRotate = true;
		m_bWorld = true;
	}

	void	Local_Rotate_Radian(const float& _X, const float& _Y, const float& _Z)
	{
		m_LocalRotate = KVector(_X, _Y, _Z);
		m_bLocalRotate = true;
		m_bWorld = true;
	}

	void Scaling(const KVector4& _Value)
	{
		m_LocalScale += _Value * DELTATIME;
		m_bLocalScale= true;
		m_bWorld = true;
	}

	// 나는 그냥 각도만 한다 - 라디안을 넣을 일이 없을 것 같다...
	void Rotating(const KVector4& _Value)
	{
		m_LocalRotate += _Value * DELTATIME * KPI / 180.0f;
		m_bLocalRotate = true;
		m_bWorld = true;
	}
	
	void Moving(const KVector4& _Value)
	{
		m_LocalPosition += _Value * DELTATIME;
		m_bLocalPostion = true;
		m_bWorld = true;
	}


	
	KVector4 World_Pos() const				{ return m_WorldPosition; }
	void	World_Pos(const KVector4& _Value)
	{
		if (nullptr != one()->transform())
		{
			m_LocalPosition = _Value - one()->Local_Pos();
		}
		else
		{
			m_LocalPosition = _Value;
		}

		m_WorldPosition = _Value;
		m_bWorld = true;
	}

	KVector4 World_Rotate() const			{ return m_WorldRotate; }
	void	World_Rotate(const KVector4& _Value)
	{
		// 일단 라디안 기준 계산
		if (nullptr != one()->parent())
		{
			// 회전은 단순히 부머에서 바뀌는 형식
			m_LocalRotate = (m_WorldRotate * KMath::DegToRad) - one()->transform()->World_Rotate();
		}
		else
		{
			m_LocalRotate = _Value * KMath::DegToRad;
		}
				
		m_bLocalRotate = true;
		m_bWorld = true;
	}


	KVector4 World_Scale() const { return m_WorldScale; }
	const KVector4& World_Scale_Const() const { return m_WorldScale; }
	void	World_Scale(const KVector4& _Value)
	{
		// 일단 라디안 기준 계산
		if (nullptr != one()->parent())
		{
			// 문제는 크기가 바뀌면 위치도 바뀌는데 그건 차후에 잡는 걸로 ->
			// 부모 자식간에 위치 조절을 한번에 잡을 걸로 예상
			// 언젠간 해야함 자식 관계가 반드시 필요하기 때문ㅇ
			m_LocalScale = m_WorldScale - one()->transform()->World_Scale();
		}
		else
		{
			m_LocalScale = _Value;
		}

		m_bLocalScale = true;
		m_bWorld = true;
	}



	void	World_Pos(const float& _X, const float& _Y, const float& _Z)
	{
		World_Pos(KVector(_X, _Y, _Z));
	}
	void	World_Scale(const float& _X, const float& _Y, const float& _Z)
	{
		World_Scale(KVector(_X, _Y, _Z));
	}
	void	World_Rotate(const float& _X, const float& _Y, const float& _Z)
	{
		World_Rotate(KVector(_X, _Y, _Z));
	}

	void	World_Pos(const KVector2& _Vec, const float& _Z)
	{
		World_Pos(KVector(_Vec, _Z));
	}
	void	World_Scale(const KVector2& _Vec, const float& _Z)
	{
		World_Scale(KVector(_Vec, _Z));
	}
	void	World_Rotate(const KVector2& _Vec, const float& _Z)
	{
		World_Rotate(KVector(_Vec, _Z));
	}
	
	KVector4& Local_Forward() { return m_LocalAx[Ax_Forward]; }
	KVector4& World_Forward() { return m_WorldAx[Ax_Forward]; }
	KVector4& Local_Up()		{ return m_LocalAx[Ax_Up]; }
	KVector4& World_Up()		{ return m_WorldAx[Ax_Up]; }
	KVector4& Local_Right()	{ return m_LocalAx[Ax_Right]; }
	KVector4& World_Right()	{ return m_WorldAx[Ax_Right]; }

	KVector4 Local_Back() { return -m_LocalAx[Ax_Forward] ; }
	KVector4 World_Back() { return -m_WorldAx[Ax_Forward] ; }
	KVector4 Local_Down() { return -m_LocalAx[Ax_Up]; }
	KVector4 World_Down() { return -m_WorldAx[Ax_Up]; }
	KVector4 Local_Left() { return -m_LocalAx[Ax_Right] ; }
	KVector4 World_Left() { return -m_WorldAx[Ax_Right] ; }




	KMatrix World_Matrix() const { return m_WorldMat; }
	const KMatrix& World_Matrix_Const() const { return m_WorldMat; }
	


	KMatrix Scale_Matrix() const { return m_ScaleMat; }
	const KMatrix& Rotate_Matrix_Const() const { return m_ScaleMat; }

	KMatrix Rotate_Matrix() const { return m_RotMat; }
	const KMatrix& Rotate_Matrix_Const() const { return m_RotMat; }

	KMatrix Position_Matrix() const { return m_PosMat; }
	const KMatrix& World_Matrix_Const() const { return m_PosMat; }



	bool BWorld() const { return m_bWorld; }
	void BWorld(const bool& _Value) { m_bWorld = _Value; }

	bool BLocalScale() const { return m_bLocalScale; }
	void BLocalScale(const bool& _Value) { m_bLocalScale = _Value; }

	bool BLocalRotate() const { return m_bLocalRotate; }
	void BLocalRotate(const bool& _Value) { m_bLocalRotate = _Value; }

	bool BLocalPostion() const { return m_bLocalPostion; }
	void BLocalPostion(const bool& _Value) { m_bLocalPostion = _Value; }

	// Polygon
	void Col_Vertex(std::vector<KVector2>* _Value)
	{
		m_VertexVec = _Value;
	}
	std::vector<KVector2>* Col_Vertex()
	{
		return m_VertexVec;
	}


public:
	virtual bool Init() override;
	virtual void FinalUpdate() override;
	virtual void End_Update() override;
	
public:
	TransPosition();
	~TransPosition();
};

