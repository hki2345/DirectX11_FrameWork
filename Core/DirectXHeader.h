#pragma once
#include <d3d11.h>  // 메인헤더
#include <d3dcompiler.h> // 쉐이더용 디버깅헤더
#include <DirectXPackedVector.h> // 다이렉트에서 쓰는 벡터따위의 단위 뭐 그런 함수들
#include "KMacro.h"


// 라이브러리 이어주는 거
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")


#define KPI 3.14159265358979323846264338327950288f

class int_compare
{
public:
	union
	{
		struct
		{
			int X;
			int Y;
		};

		struct
		{
			int Left;
			int Right;
		};

		__int64 m_i64;
	};

public:
	operator __int64()
	{
		return *((__int64*)&X);
	}

public:
	int_compare(int _X, int _Y) : X(_X), Y(_Y) {}
	int_compare(__int64 _Value) : m_i64(_Value) {}
	int_compare() {}
};

class KVector2
{

public:
	// 이부분은 사실 시험용 벡터를 구현하면서 나왔던 내용임ㅇㅇ
	// 이름 없는 공용체 -> 선언없이 바로 추가--- 구조체와 같은 맥락
	union
	{
		// 다이렉트X 에선 Vector3형식을 지원하는 것이 있으나
		// 작용하는 벡터는 이 벡터(x, y, z, w)로 반환한다.
		// 따라서 이 벡터를 멤버로 가지고 있는 것임
		DirectX::XMFLOAT2 m_Vector;
		float s[2];

		// 이름없는 구조체 -> 선언없이 바로 추가해서 인자값에 접근가능
		// 이너클래스 따위의 구조체가 아님
		struct
		{
			float x;
			float y;
		};

		struct
		{
			float m1;
			float m2;
		};

		struct
		{
			int ix;
			int iy;
		};

		__int64 m_i64;
	};

public:
	KVector2() : m_Vector(.0f, .0f) {}
	KVector2(__int64 _Value) : m_i64(_Value) {}
	KVector2(float _Value) : m_Vector(_Value, _Value) {}
	KVector2(float _x, float _y) : m_Vector(_x, _y) {}
	KVector2(int _x, int _y) : ix(_x), iy(_y) {}
	KVector2(const KVector2& _Other) : m_Vector(_Other.x, _Other.y) {}

	// 벡터에서 그때배운 플롯 4개를 리틀에디안 해 가장 빠르게 연산 할 수 있게 저장 아예 저장단계에서 부터 ㅇㅇ
	KVector2(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat2(&m_Vector, _Other); }


public:
	static const KVector2 Zero;
	static const KVector2 Left;
	static const KVector2 Right;
	static const KVector2 Up;
	static const KVector2 Down;


	void set_vector2(const float& _X, const float& _Y) { x = _X; y = _Y; }
	float x_part() const { return x * .5f; }
	float y_part() const { return y * .5f; }

	KVector2 IntToFloat() const
	{
		return KVector2((float)ix, (float)iy);
	}

	KVector2 FloatToInt() const
	{
		return KVector2(ix, iy);
	}

	KVector2 operator + (const KVector2& _Other)
	{
		return{ x + _Other.x, y + _Other.y };
	}

	KVector2& operator = (const KVector2& _Value)
	{
		m_Vector = _Value.m_Vector;
		return *this;
	}


	KVector2 operator + (const KVector2& _Value) const
	{
		return KVector2(x + _Value.x, y + _Value.y);
	}
	KVector2 operator * (const KVector2& _Value) const
	{
		return KVector2(x * _Value.x, y * _Value.y);
	}
	KVector2 operator - (const KVector2& _Value) const
	{
		return KVector2(x - _Value.x, y - _Value.y);
	}
	KVector2 operator / (const KVector2& _Value) const
	{
		return KVector2(x / _Value.x, y / _Value.y);
	}

	KVector2 operator + (const float& _Value) const
	{
		return KVector2(x + _Value, y + _Value);
	}
	KVector2 operator * (const float& _Value)const
	{
		return KVector2(x * _Value, y * _Value);
	}
	KVector2 operator - (const float& _Value)const
	{
		return KVector2(x - _Value, y - _Value);
	}
	KVector2 operator / (const float& _Value)const
	{
		return KVector2(x / _Value, y / _Value);
	}



	KVector2& operator += (const KVector2& _Value)
	{
		x += _Value.x;
		y += _Value.y;

		return *this;
	}
	KVector2& operator -= (const KVector2& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;

		return *this;
	}
	KVector2& operator *= (const KVector2& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;

		return *this;
	}
	KVector2& operator /= (const KVector2& _Value)
	{
		x /= _Value.x;
		y /= _Value.y;

		return *this;
	}

	bool operator == (const KVector2& _Value)
	{
		return (x == _Value.x && y == _Value.y);
	}
	bool operator != (const KVector2& _Value)
	{
		return (x != _Value.x && y != _Value.y);
	}


	bool operator == (const KVector2& _Value) const
	{
		return (x == _Value.x && y == _Value.y);
	}
	bool operator != (const KVector2& _Value) const
	{
		return (x != _Value.x || y != _Value.y);
	}
	bool operator >= (const KVector2& _Value) const
	{
		return (x >= _Value.x && y >= _Value.y);
	}
	bool operator <= (const KVector2& _Value) const
	{
		return (x <= _Value.x && y <= _Value.y);
	}
	bool operator > (const KVector2& _Value) const
	{
		return (x > _Value.x && y > _Value.y);
	}
	bool operator < (const KVector2& _Value) const
	{
		return (x < _Value.x && y < _Value.y);
	}



	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat2(&m_Vector);
	}

	operator __int64() const
	{
		return m_i64;
	}


	float length() const
	{
		return sqrtf(x*x + y*y);
	}

	float distance() const
	{
		return sqrtf((x) * (x) + (y) * (y));
	}

	float distance(const KVector2& _Value) const
	{
		return sqrtf((x - _Value.x) * (x - _Value.x) + (y - _Value.y) * (y - _Value.y));
	}

	void normalize()
	{
		*this = DirectX::XMVector2Normalize(*this);
	}

	const float dot(const KVector2& _Left, const KVector2& _Right)
	{
		KVector2 Value = DirectX::XMVector2Dot(_Left, _Right);
		return Value.x;
	}

	float dot(const KVector2& _Other) const
	{
		return x * _Other.x + y * _Other.y;
	}

	KVector2& y_reverse()
	{
		y *= -1.0f;
		return *this;
	}

	float x_ratio() const
	{
		if (0 > y)
		{
			return .0f;
		}

		return x / y;
	}
	float y_ratio() const
	{
		if (0 > x)
		{
			return .0f;
		}

		return y / x;
	}


};
typedef KVector2 KPoint;


class KVector3
{
public:
	// 이부분은 사실 시험용 벡터를 구현하면서 나왔던 내용임ㅇㅇ
	// 이름 없는 공용체 -> 선언없이 바로 추가--- 구조체와 같은 맥락
	union
	{
		// 다이렉트X 에선 Vector3형식을 지원하는 것이 있으나
		// 작용하는 벡터는 이 벡터(x, y, z, w)로 반환한다.
		// 따라서 이 벡터를 멤버로 가지고 있는 것임
		DirectX::XMFLOAT3 m_Vector;

		float s[3];

		// 이름없는 구조체 -> 선언없이 바로 추가해서 인자값에 접근가능
		// 이너클래스 따위의 구조체가 아님
		struct
		{
			KVector2 m_Pos;
			float Radius;
		};

		struct
		{
			float x;
			float y;
			float z;
		};
		
		struct
		{
			float m1;
			float m2;
			float m3;
		};

		struct
		{
			KVector2 m_Vector2;
			float Value;
		};
	};

public:
	KVector3() : m_Vector(.0f, .0f, .0f) {}
	KVector3(float _Value) : m_Vector(_Value, _Value, _Value) {}
	KVector3(float _x, float _y, float _z) : m_Vector(_x, _y, _z) {}
	KVector3(const KVector2& _Other, float _z) : m_Vector(_Other.x, _Other.y, _z) {}
	KVector3(const KVector3& _Other) : m_Vector(_Other.x, _Other.y, _Other.z) {}

	// 벡터에서 그때배운 플롯 4개를 리틀에디안 해 가장 빠르게 연산 할 수 있게 저장 아예 저장단계에서 부터 ㅇㅇ
	KVector3(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat3(&m_Vector, _Other); }


public:
	static const KVector3 Zero;
	static const KVector3 Left;
	static const KVector3 Right;
	static const KVector3 Up;
	static const KVector3 Down;
	static const KVector3 Forword;
	static const KVector3 Back;
	static const KVector3 One; // 색깔지정ㅇㅇㅇ


	void set_vector3(const float& _X, const float& _Y, const float& _Z) { x = _X; y = _Y; z = _Z; }
	float x_part() { return x * .5f; }
	float y_part() { return y * .5f; }
	float z_part() { return z * .5f; }

	KVector3 operator + (const KVector3& _Other)
	{
		return{ x + _Other.x, y + _Other.y , z + _Other.z};
	}

	KVector3& operator = (const KVector3& _Value)
	{
		m_Vector = _Value.m_Vector;
		return *this;
	}

	KVector3& operator = (const DirectX::XMVECTOR& _Value)
	{
		DirectX::XMStoreFloat3(&m_Vector, _Value);
		return *this;
	}
	KVector3 operator + (const KVector3& _Value) const
	{
		return KVector3(x + _Value.x, y + _Value.y, z + _Value.z);
	}
	KVector3 operator * (const KVector3& _Value) const
	{
		return KVector3(x * _Value.x, y * _Value.y, z * _Value.z);
	}
	KVector3 operator - (const KVector3& _Value) const
	{
		return KVector3(x - _Value.x, y - _Value.y, z - _Value.z);
	}
	KVector3 operator / (const KVector3& _Value) const
	{
		return KVector3(x / _Value.x, y / _Value.y, z / _Value.z);
	}

	KVector3 operator + (const float& _Value) const
	{
		return KVector3(x + _Value, y + _Value, z + _Value);
	}
	KVector3 operator * (const float& _Value)const
	{
		return KVector3(x * _Value, y * _Value, z * _Value);
	}
	KVector3 operator - (const float& _Value)const
	{
		return KVector3(x - _Value, y - _Value, z - _Value);
	}
	KVector3 operator / (const float& _Value)const
	{
		return KVector3(x / _Value, y / _Value, z / _Value);
	}



	KVector3& operator += (const KVector3& _Value)
	{
		x += _Value.x;
		y += _Value.y;
		z += _Value.z;

		return *this;
	}
	KVector3& operator -= (const KVector3& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;
		z -= _Value.z;

		return *this;
	}
	KVector3& operator *= (const KVector3& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;
		z *= _Value.z;

		return *this;
	}
	KVector3& operator /= (const KVector3& _Value)
	{
		x /= _Value.x;
		y /= _Value.y;
		z /= _Value.z;

		return *this;
	}

	bool operator == (const KVector3& _Value) const
	{
		return (x == _Value.x && y == _Value.y && z == _Value.z);
	}
	bool operator != (const KVector3& _Value) const
	{
		return (x != _Value.x && y != _Value.y && z != _Value.z);
	}


	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat3(&m_Vector);
	}

	const KVector3 dot(const KVector3& _Left, const KVector3& _Right)
	{
		KVector3 Value = DirectX::XMVector3Dot(_Left, _Right);
		return Value.x;
	}

	float dot(const KVector3& _Other) const
	{
		KVector3 Value = DirectX::XMVector3Dot(*this, _Other);
		return Value.x;
	}

	// 당연히 차원이 높아진다.ㅇㅇㅇㅇㅇㅇㅇㅇ
	KVector3 cross(const KVector2& _Left, const KVector2& _Right) const
	{
		return DirectX::XMVector3Cross(_Left, _Right);
	}

	// 당연히 차원이 높아진다.ㅇㅇㅇㅇㅇㅇㅇㅇ
	KVector3 cross(const KVector2& _Right) const
	{
		return DirectX::XMVector3Cross(*this, _Right);
	}


	// 당연히 차원이 높아진다.ㅇㅇㅇㅇㅇㅇㅇㅇ
	KVector3 cross(const KVector3& _Right) const
	{
		KVector3 Temp = *this * KPI / 180;
		return DirectX::XMVector3Cross(Temp, _Right);
	}

	// 3차원 벡터 거리 공식 -- 원점에서 벡터 -> 3차원 두 좌표에서의 거리
	float length() const
	{
		return sqrtf(x*x + y*y + z*z);
	}

	float diameter() const
	{
		return Radius * 2.0f;
	}

	KVector3& normalize()
	{
		return *this = DirectX::XMVector3Normalize(*this);
	}


	void Absolute()
	{
		x = fabsf(x); y = fabsf(y); z = fabsf(z);
	}
};
typedef KVector3 KCircle;

#define KRGBA(r, g, b, a) ( (COLORREF)(((BYTE)(r) | ((COLORREF)((BYTE)(g))<<8)) | ((COLORREF)(BYTE)(b))<<16 | ((COLORREF)(BYTE)(a))<<24 )

class KVector4
{
public:
	// 이부분은 사실 시험용 벡터를 구현하면서 나왔던 내용임ㅇㅇ
	// 이름 없는 공용체 -> 선언없이 바로 추가--- 구조체와 같은 맥락
	union
	{
		// 다이렉트X 에선 Vector3형식을 지원하는 것이 있으나
		// 작용하는 벡터는 이 벡터(x, y, z, w)로 반환한다.
		// 따라서 이 벡터를 멤버로 가지고 있는 것임
		DirectX::XMFLOAT4 m_Vector;
		KVector2	m_Vector2;
		KVector3	m_Vector3;

		float s[4];


		// 이름없는 구조체 -> 선언없이 바로 추가해서 인자값에 접근가능
		// 이너클래스 따위의 구조체가 아님
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float m1;
			float m2;
			float m3;
			float m4;
		};

		struct
		{
			KVector2 m_Pos;
			KVector2 m_Size;
		};

		struct
		{
			KVector2 m_Front;
			KVector2 m_Back;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
	};

public:
	static const KVector4 Left;
	static const KVector4 Right;
	static const KVector4 Up;
	static const KVector4 Down;
	static const KVector4 Forword;
	static const KVector4 Back;
	static const KVector4 Zero;
	static const KVector4 One; // 색깔지정ㅇㅇㅇ


	static const KVector4 Color_None;
	static const KVector4 White;
	static const KVector4 Black;
	static const KVector4 Red;
	static const KVector4 Green;
	static const KVector4 Blue;
	static const KVector4 Yellow;
	static const KVector4 Majenta;
	static const KVector4 Cyan;
	static const KVector4 Gray;


public:
	KVector4() : m_Vector(.0f, .0f, .0f, .0f) {}
	KVector4(float _Value) : m_Vector(_Value, _Value, _Value, _Value) {}
	KVector4(float _x, float _y) : m_Vector(_x, _y, .0f, .0f) {}
	KVector4(float _x, float _y, float _z) : m_Vector(_x, _y, _z, .0f) {}
	KVector4(float _x, float _y, float _z, float _w) : m_Vector(_x, _y, _z, _w) {}
	KVector4(float _x, float _y, const KVector2& _Vec) : m_Front(_x, _y), m_Back(_Vec) {}
	KVector4(const KVector4& _Other) : m_Vector(_Other.x, _Other.y, _Other.z, _Other.w) {}
	KVector4(const KVector2& _Other1, const KVector2& _Other2) : m_Front(_Other1), m_Back(_Other2) {}
	KVector4(const KVector3& _Other) : m_Vector(_Other.x, _Other.y, _Other.z, .0f) {}
	KVector4(const KVector2& _Other) : m_Vector(_Other.x, _Other.y, .0f, .0f) {}
	KVector4(const KVector2& _Other, const float& _V1) : m_Vector(_Other.x, _Other.y, _V1, .0f) {}
	KVector4(const KVector2& _Other, const float& _V1, const float& _V2) : m_Vector(_Other.x, _Other.y, _V1, _V2) {}

	// 벡터에서 그때배운 플롯 4개를 리틀에디안 해 가장 빠르게 연산 할 수 있게 저장 아예 저장단계에서 부터 ㅇㅇ
	KVector4(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat4(&m_Vector, _Other); }


public:
	float distance(const KVector4& _Value) const
	{
		return sqrtf((x) * (x) + (y) * (y) + (z) * (z) + (w) * (w));
	}


	static KVector4 cross3D(const KVector4& _Left, const KVector4& _Right)
	{
		KVector4 Temp = DirectX::XMVector3Cross(_Left, _Right);
		return Temp;
	}

	KVector4 cross3D(const KVector4& _Other)
	{
		KVector4 Temp = DirectX::XMVector3Cross(*this, _Other);
		return Temp;
	}

	KVector4 ElipseCut()
	{
		if (fabsf(x) < .00000001f)
		{
			x = .0f;
		}

		if (fabsf(y) < .00000001f)
		{
			y = .0f;
		}

		if (fabsf(z) < .00000001f)
		{
			z = .0f;
		}

		return KVector4(x, y, z);
	}

	float left() const { return m_Pos.x - m_Size.x_part(); }
	float right() const { return m_Pos.x + m_Size.x_part(); }
	float bottom() const { return m_Pos.y - m_Size.y_part(); }
	float top() const { return m_Pos.y + m_Size.y_part(); }

	KVector2 left_top() const { return{ left(), top() }; }
	KVector2 left_bottom() const { return{ left(), bottom() }; }
	KVector2 right_top() const { return{ right(), top() }; }
	KVector2 right_bottom() const { return{ right(), bottom() }; }

	float x_part() { return x * .5f; }
	float y_part() { return y * .5f; }
	float z_part() { return z * .5f; }
	float w_part() { return w * .5f; }

	UINT color_to_reverse255() const
	{
		return KRGBA((UINT)(r * 255), (UINT)(g * 255), (UINT)(b * 255), (UINT)(a * 255)));
	}

	KVector4& operator = (const KVector4& _Value)
	{
		m_Vector = _Value.m_Vector;
		return *this;
	}

	KVector4& operator = (const KVector3& _Value)
	{
		m_Vector3 = _Value;
		return *this;
	}

	KVector4& operator = (const DirectX::XMVECTOR& _Value)
	{
		DirectX::XMStoreFloat4(&m_Vector, _Value);
		return *this;
	}

	KVector4& operator = (const float& _Value)
	{
		m_Vector = { _Value, _Value, _Value, _Value };
		return *this;
	}

	KVector4 operator -()
	{
		return KVector4(-x, -y, -z, -w);
	}


	KVector4 operator + (const KVector4& _Value) const
	{
		return KVector4(x + _Value.x, y + _Value.y, z + _Value.z, w + _Value.w);
	}
	KVector4 operator * (const KVector4& _Value) const
	{
		return KVector4(x * _Value.x, y * _Value.y, z * _Value.z, w * _Value.w);
	}
	KVector4 operator - (const KVector4& _Value) const
	{
		return KVector4(x - _Value.x, y - _Value.y, z - _Value.z, w - _Value.w);
	}
	KVector4 operator / (const KVector4& _Value) const
	{
		KVector4 temp;

		if (0 != _Value.x) { temp.x = x / _Value.x; }
		else { temp.x; }
		if (0 != _Value.y) { temp.y = y / _Value.y; }
		else { temp.y; }
		if (0 != _Value.z) { temp.z = z / _Value.z; }
		else { temp.z; }
		if (0 != _Value.w) { temp.w = w / _Value.w; }
		else { temp.w; }

		return temp;
	}

	KVector2 operator + (const KVector2& _Value) const
	{
		return KVector2(x + _Value.x, y + _Value.y);
	}
	KVector2 operator * (const KVector2& _Value) const
	{
		return KVector2(x * _Value.x, y * _Value.y);
	}
	KVector2 operator - (const KVector2& _Value) const
	{
		return KVector2(x - _Value.x, y - _Value.y);
	}
	KVector2 operator / (const KVector2& _Value) const
	{
		KVector2 temp;

		if (0 != _Value.x) { temp.x = x / _Value.x; }
		else { temp.x; }
		if (0 != _Value.y) { temp.y = y / _Value.y; }
		else { temp.y; }

		return temp;
	}

	KVector4 operator + (const float& _Value) const
	{
		return KVector4(x + _Value, y + _Value, z + _Value);
	}
	KVector4 operator * (const float& _Value)const
	{
		return KVector4(x * _Value, y * _Value, z * _Value);
	}
	KVector4 operator - (const float& _Value)const
	{
		return KVector4(x - _Value, y - _Value, z - _Value);
	}
	KVector4 operator / (const float& _Value)const
	{
		return KVector4(x / _Value, y / _Value, z / _Value);
	}



	KVector4& operator += (const float& _Value)
	{
		x += _Value;
		y += _Value;
		z += _Value;
		w += _Value;

		return *this;
	}
	KVector4& operator -= (const float& _Value)
	{
		x -= _Value;
		y -= _Value;
		z -= _Value;
		w -= _Value;

		return *this;
	}
	KVector4& operator *= (const float& _Value)
	{
		x *= _Value;
		y *= _Value;
		z *= _Value;
		w *= _Value;

		return *this;
	}
	KVector4& operator /= (const float& _Value)
	{
		{ x /= _Value; }
		{ y /= _Value; }
		{ z /= _Value; }
		{ w /= _Value; }

		return *this;
	}


	KVector4& operator += (const KVector4& _Value)
	{
		x += _Value.x;
		y += _Value.y;
		z += _Value.z;
		w += _Value.w;

		return *this;
	}
	KVector4& operator -= (const KVector4& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;
		z -= _Value.z;
		w -= _Value.w;

		return *this;
	}
	KVector4& operator *= (const KVector4& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;
		z *= _Value.z;
		w *= _Value.w;

		return *this;
	}
	KVector4& operator /= (const KVector4& _Value)
	{
		{ x /= _Value.x; }
		{ y /= _Value.y; }
		{ z /= _Value.z; }
		{ w /= _Value.w; }

		return *this;
	}

	bool operator == (const KVector4& _Value)
	{
		return (x == _Value.x && y == _Value.y && z == _Value.z && w == _Value.w);
	}
	bool operator != (const KVector4& _Value) const
	{
		return (x != _Value.x || y != _Value.y || z != _Value.z || w != _Value.w);
	}
	bool operator >= (const KVector4& _Value)
	{
		return (x == _Value.x && y == _Value.y && z == _Value.z && w == _Value.w);
	}
	bool operator <= (const KVector4& _Value) const
	{
		return (x != _Value.x || y != _Value.y || z != _Value.z || w != _Value.w);
	}

	operator KVector2() const
	{
		return{ m_Vector.x, m_Vector.y };
	}

	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&m_Vector);
	}

public:
	static KVector4 Dot(const KVector4& _Left, const KVector4& _Right)
	{
		KVector4 Value = DirectX::XMVector4Dot(_Left, _Right);
		return Value.x;
	}

	float Dot(const KVector4& _Other) const
	{
		KVector4 Value = DirectX::XMVector4Dot(*this, _Other);
		return Value.x;
	}

	void Normalize()
	{
		*this = DirectX::XMVector4Normalize(*this);
	}

	void NormalizeVec3()
	{
		*this = DirectX::XMVector3Normalize(*this);
		w = .0f;
	}
	void NormalizeVec2()
	{
		*this = DirectX::XMVector2Normalize(*this);
		z = .0f;
		w = .0f;
	}

	KVector3 Vec3lize()
	{
		return KVector3(x, y, z);
	}
};

typedef KVector4 KColor;
typedef KVector4 KRect;
typedef KVector4 KVector;



class KMatrix
{
public:
	union
	{
		DirectX::XMFLOAT4X4 m_Mat;

		// 열단위로 가지고 있기 위함
		struct
		{
			KVector4 vec1;
			KVector4 vec2;
			KVector4 vec3;
			KVector4 vec4;
		};

		float m[4][4];
	};

public:
	KMatrix() 	{}

	KMatrix(const KMatrix& _Other)
	{
		memcpy_s(this, sizeof(KMatrix), &_Other, sizeof(KMatrix));
	}

	KMatrix(const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&m_Mat, _Other);
	}


public:
	operator DirectX::XMMATRIX() const
	{
		return DirectX::XMLoadFloat4x4(&m_Mat);
	}


	KMatrix& operator= (const KMatrix& _Other)
	{
		memcpy_s(this, sizeof(KMatrix), &_Other, sizeof(KMatrix));
		return *this;
	}

	KMatrix& operator= (const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&m_Mat, _Other);
		return *this;
	}

	KMatrix operator*(const DirectX::XMMATRIX& _Value) const
	{
		return DirectX::XMMatrixMultiply(*this, _Value);
	}

public:

	KMatrix WZero()
	{
		KMatrix Temp = *this;
		Temp.vec4 = KVector4::Zero;
		return Temp;
	}

	KMatrix WOne()
	{
		KMatrix Temp = *this;
		Temp.vec4 = KVector4::One;
		return Temp;
	}

	KMatrix Multiply3X3(const KMatrix& _Other) const
	{
		KMatrix Temp = *this * _Other;

		Temp.vec4 = (*this).vec4;

		Temp.vec1.w = (*this).vec1.w;
		Temp.vec2.w = (*this).vec2.w;
		Temp.vec3.w = (*this).vec3.w;
				
		return Temp;
	}

	KMatrix& Identity()
	{
		*this = DirectX::XMMatrixIdentity();
		return *this;
	}

	void Scale(const KVector4& _Scale)
	{
		*this = DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
		return;
	}

	void Rotate_X(float _Value)
	{
		*this = DirectX::XMMatrixRotationX(_Value);
		return;
	}

	void Rotate_Y(float _Value)
	{
		*this = DirectX::XMMatrixRotationY(_Value);
		return;
	}

	void Rotate_Z(float _Value)
	{
		*this = DirectX::XMMatrixRotationZ(_Value);
		return;
	}

	void Translation(const KVector4& _Value)
	{
		*this = DirectX::XMMatrixTranslation(_Value.x, _Value.y, _Value.z);
		return;
	}

	// 전치행렬 - 대각 기준 대칭
	void TransPose()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return;
	}

	// 포인트를 반환해주는 행렬 - 역행렬
	const KMatrix& Inverse_Referance()
	{
		// 행렬의 행렬식을 반환한다.
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(*this);
		*this = DirectX::XMMatrixInverse(&det, *this);
		return *this;
	}

	// 값을 반환해주는 행렬 - 역행렬
	KMatrix Inverse_Value() const
	{
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(*this);
		return DirectX::XMMatrixInverse(&det, *this);
	}

	// 포인트를 반환해주는 행렬 - 전치 행렬
	const KMatrix& TransPose_Referance()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return *this;
	}

	// 값을 반환해주는 행렬 - 전치 행렬
	KMatrix TransPose_Value() const
	{
		return DirectX::XMMatrixTranspose(*this);
	}

	// ViewAt -> 월드에 어느좌표로 볼것인지 -> 안에서 방향벡터를 구함
	// 위치와 해당 위치를 빼서 방향벡터를 구함
	void ViewAt_LH(const KVector4& _CamPos, const KVector4& _LookWorldPos, const KVector4& _Up)
	{
		*this = DirectX::XMMatrixLookAtLH(_CamPos, _LookWorldPos, _Up);
		return;
	}

	// 방향벡터를 바로 넣어 보게함
	void ViewTo_LH(const KVector4& _CamPos, const KVector4& _LookDirPos, const KVector4& _Up)
	{
		*this = DirectX::XMMatrixLookToLH(_CamPos, _LookDirPos, _Up);
		return;
	}


	// 투영 - 원근
	void Proj_Perspective(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width / _Height, _Near, _Far);
		return;
	}

	// 투영 - 직교
	void Proj_Orthographic(float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
		return;
	}

	// 위치 속성 적용 O
	KVector4 Multi_Vector_O(const KVector4& _Value) const
	{
		return DirectX::XMVector3TransformCoord(_Value, *this);
	}

	// 위치 속성 적용 X
	KVector4 Multi_Vector_Z(const KVector4& _Value) const
	{
		return DirectX::XMVector3TransformNormal(_Value, *this);
	}
};

class DATA_3D
{
public:
	KMatrix m_W;
	KMatrix m_V;
	KMatrix m_P;
	KMatrix m_WV;
	KMatrix m_WVP;
};

class VertShaderFormat
{
public:
	static UINT size_Formet(DXGI_FORMAT _Form);
};


enum SHADER_TYPE
{
	ST_NONE,
	ST_VS,
	ST_PS,
};