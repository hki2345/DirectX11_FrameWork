#pragma once
#include <Windows.h>

class Vector3
{
public:
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
			float W;
		};

		float m[3];
	};


public:
	int x_int() {
		return (int)X;
	}

	int y_int() {
		return (int)Y;
	}

	int z_int() {
		return (int)Z;
	}

	Vector3& operator *= (const float _Value) {

		X *= _Value;
		Y *= _Value;
		Z *= _Value;
		W *= _Value;

		return *this;
	}

	Vector3& operator /= (const float _Value) {

		X /= _Value;
		Y /= _Value;
		Z /= _Value;
		W /= _Value;

		return *this;
	}


	Vector3 operator - (const Vector3& _Value) const
	{
		return Vector3(X - _Value.X, Y - _Value.Y, Z - _Value.Z);
	}

	void Normalize()
	{
		float line = sqrt((X * X) + (Y * Y) + (Z * Z));
		X = X / line;
		Y = Y / line;
		Z = Z / line;
		W = W / line;
	}

public:
	float NaeJuck(const Vector3& _Value) const
	{
		return (X * _Value.X) + (Y * _Value.Y) + (Z * _Value.Z);
	}
	Vector3 WaeJuck(const Vector3& _Value) const
	{
		return Vector3(
			Y * _Value.Z - Z * _Value.Y,
			X * _Value.Z - Z * _Value.X,
			X * _Value.Y - Y * _Value.X);
	}

public:
	Vector3() : X(0.0F), Y(0.0F), Z(0.0F), W(.0f)
	{
	}

	Vector3(float _X, float _Y, float _Z) : X(_X), Y(_Y), Z(_Z), W(.0f)
	{

	}
	~Vector3() {};
};

// 행렬
// 행렬 순서
// 로컬 -> 월드 -> 뷰 -> (후면 추려내기) -> (조명) -> (클리핑) -> 투영 -> 뷰포트 -> (레스터라이즈)
class Matrix
{

public:
	union
	{
		float m[4][4];
	};

public:
	// 항등 행렬
	void identity()
	{
		memset(m, 0, sizeof(float) * 16);

		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
	}

	// 위치행렬
	// 1***
	// *1**
	// **1*
	// xyzw
	void Move(const Vector3& _Move)
	{
		identity();
		
		m[3][0] = _Move.X;
		m[3][1] = _Move.Y;
		m[3][2] = _Move.Z;
	}

	// 크기행렬
	// x***
	// *y**
	// **z*
	// ***1
	void Scale(const Vector3& _Scale)
	{
		m[0][0] = _Scale.X;
		m[1][1] = _Scale.Y;
		m[2][2] = _Scale.Z;
	}

	// X 회전
	// * *    *   *
	// * cos  sin *
	// * -sin cos *
	// * *    *   1
	void Rotate_AxisX(const float& _Rotate)
	{
		identity();
		float Rad = _Rotate * KPI / 180;

		m[1][1] = cos(Rad);
		m[1][2] = -sin(Rad);

		m[2][1] = sin(Rad);
		m[2][2] = cos(Rad);
	}

	// Y 회전
	// cos  *  -sin *
	// *    *  *    *
	// sin  *  cos  *
	// *    *  *    1
	void Rotate_AxisY(const float& _Rotate)
	{
		identity();
		float Rad = _Rotate * KPI / 180;

		m[0][0] = cos(Rad);
		m[0][2] = sin(Rad);

		m[2][0] = -sin(Rad);
		m[2][2] = cos(Rad);
	}

	// Z 회전
	// cos  sin *  *
	// -sin cos *  *
	// *    *   *  *
	// *    *   *  1
	void Rotate_AxisZ(const float& _Rotate)
	{
		identity();
		float Rad = _Rotate * KPI / 180;

		m[0][0] = cos(Rad);
		m[0][1] = -sin(Rad);

		m[1][0] = sin(Rad);
		m[1][1] = cos(Rad);
	}

#pragma region Revolution
	void Revolution_AxisX(const float& _Rotate, const Vector3& _Value)
	{
		identity();
		float Rad = _Rotate * KPI / 180;

		m[1][1] = cos(Rad);
		m[1][2] = -sin(Rad);

		m[2][1] = sin(Rad);
		m[2][2] = cos(Rad);
	}
	void Revolution_AxisY(const float& _Rotate, const Vector3& _Value)
	{
		identity();
		float Rad = _Rotate * KPI / 180;

		m[0][0] = cos(Rad);
		m[0][2] = sin(Rad);

		m[2][0] = -sin(Rad);
		m[2][2] = cos(Rad);
	}
	void Revolution_AxisZ(const float& _Rotate)
	{
		identity();
		float Rad = _Rotate * KPI / 180;

		m[0][0] = cos(Rad);
		m[0][1] = -sin(Rad);

		m[1][0] = sin(Rad);
		m[1][1] = cos(Rad);
	}

#pragma endregion

	// 뷰 행렬 - 카메라 행렬
	// 정점의 위치 - 카메라 위치
	// 카메라 위치 , 어떤 정점, 카메라 방향, 카메라 꼭다리
	void View(const Vector3& _Eye, const Vector3& _Pos, const Vector3& _LookDir, const Vector3& _Up)
	{
		identity();

		// 카메라의 축을 구한다.
		Vector3 xAxis;
		Vector3 yAxis;
		Vector3 zAxis;

		zAxis = _LookDir - _Pos;
		zAxis.Normalize();
		xAxis = zAxis.WaeJuck(_Up);
		xAxis.Normalize();

		// 왼손 좌표계 -> 오른손 일 경우 둘이 바뀌어야 한다.
		yAxis = xAxis.WaeJuck(zAxis);

		// 행렬 작성
		m[0][0] = xAxis.X;
		m[0][1] = yAxis.X;
		m[0][2] = zAxis.X;

		m[1][0] = xAxis.Y;
		m[1][1] = yAxis.Y;
		m[1][2] = zAxis.Y;

		m[2][0] = xAxis.Z;
		m[2][1] = yAxis.Z;
		m[2][2] = zAxis.Z;

		m[3][0] = -1.0f * xAxis.NaeJuck(_Eye);
		m[3][0] = -1.0f * yAxis.NaeJuck(_Eye);
		m[3][0] = -1.0f * zAxis.NaeJuck(_Eye);

		m[3][3] = 1.0f;
	}

	// 투영 - 원근
	// W***
	// *H**
	// **A*
	// **B0
	// aspect 종횡비
	void Projection(const float& _fovY, const float& _aspect, const float& _near, const float& _far)
	{
		identity();

		// 종횡비
		float As = _aspect;

		// 60도 -> FOV 시야각
		float F = KPI / 3.0f;

		float H = 1 / tanf(F);/*2.0f * _near / 1024.0f*/;
		float W = H / As; //2.0f * _near / 768.0f; // -> AS = aspect :: 이 함수 내에서 계산을 해준다.ㅇ
		float A = _near / (_far - _near);
		float B = _near * A;

		m[0][0] = W;
		m[1][1] = H;
		m[2][2] = A;
		m[3][2] = B;
		
		m[2][3] = 1.0f;
		m[3][3] = .0f;
	}

	// 뷰포트 행렬
	// W***
	// *H**
	// **A*
	// **B0
	void ViewPort()
	{
		identity();

		m[0][0] = 1024.0f * .5f;
		m[1][1] = -768.0f * .5f;;
		m[2][2] = 999.7f;
		m[3][3] = 1.0f;

		m[3][0] = 1024.0f * .5f;
		m[3][1] = 768.0f * .5f;
		m[3][2] = .4f;
	}

#pragma region Operator

	// 숙제 -> 행렬과 행렬을 for구문으로 구현하라
	Matrix operator*(const Matrix& _Value)
	{
		Matrix Temp;
		Temp.identity();

		for (size_t y = 0; y < 4; y++)
		{
			for (size_t x = 0; x < 4; x++)
			{
				Temp.m[y][x] =
					m[y][0] * _Value.m[0][x] +
					m[y][1] * _Value.m[1][x] +
					m[y][2] * _Value.m[2][x] +
					m[y][3] * _Value.m[3][x];
			}
		}

		return Temp;
	}

	// 벡터에 해당 행렬을 곱해준다. -> 벡터 * 행렬
	Vector3 Multi_WO(const Vector3& _Value)
	{
		Vector3 temp;

		// y < 3; -> w까지 있으면 4가 맞다
		for (size_t y = 0; y < 4; ++y)
		{
			for (size_t x = 0; x < 4; ++x)
			{
				temp.m[y] += m[x][y] * _Value.m[x];
			}
		}

		return temp;
	}

	Vector3 Multi_WZ(const Vector3& _Value)
	{
		Vector3 temp;

		// y < 3; -> w까지 있으면 4가 맞다
		for (size_t y = 0; y < 4; ++y)
		{
			for (size_t x = 0; x < 4; ++x)
			{
				temp.m[y] += m[x][y] * _Value.m[x];
			}
		}

		return temp;
	}

#pragma endregion

public:
	Matrix()
	{
		identity();
	}

	~Matrix() {};
};
