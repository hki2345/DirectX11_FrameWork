#pragma once
#include <d3d11.h>  // �������
#include <d3dcompiler.h> // ���̴��� ��������
#include <DirectXPackedVector.h> // ���̷�Ʈ���� ���� ���͵����� ���� �� �׷� �Լ���
#include "KMacro.h"


// ���̺귯�� �̾��ִ� ��
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
	// �̺κ��� ��� ����� ���͸� �����ϸ鼭 ���Դ� �����Ӥ���
	// �̸� ���� ����ü -> ������� �ٷ� �߰�--- ����ü�� ���� �ƶ�
	union
	{
		// ���̷�ƮX ���� Vector3������ �����ϴ� ���� ������
		// �ۿ��ϴ� ���ʹ� �� ����(x, y, z, w)�� ��ȯ�Ѵ�.
		// ���� �� ���͸� ����� ������ �ִ� ����
		DirectX::XMFLOAT2 m_Vector;
		float s[2];

		// �̸����� ����ü -> ������� �ٷ� �߰��ؼ� ���ڰ��� ���ٰ���
		// �̳�Ŭ���� ������ ����ü�� �ƴ�
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

	// ���Ϳ��� �׶���� �÷� 4���� ��Ʋ����� �� ���� ������ ���� �� �� �ְ� ���� �ƿ� ����ܰ迡�� ���� ����
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
	// �̺κ��� ��� ����� ���͸� �����ϸ鼭 ���Դ� �����Ӥ���
	// �̸� ���� ����ü -> ������� �ٷ� �߰�--- ����ü�� ���� �ƶ�
	union
	{
		// ���̷�ƮX ���� Vector3������ �����ϴ� ���� ������
		// �ۿ��ϴ� ���ʹ� �� ����(x, y, z, w)�� ��ȯ�Ѵ�.
		// ���� �� ���͸� ����� ������ �ִ� ����
		DirectX::XMFLOAT3 m_Vector;

		float s[3];

		// �̸����� ����ü -> ������� �ٷ� �߰��ؼ� ���ڰ��� ���ٰ���
		// �̳�Ŭ���� ������ ����ü�� �ƴ�
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

	// ���Ϳ��� �׶���� �÷� 4���� ��Ʋ����� �� ���� ������ ���� �� �� �ְ� ���� �ƿ� ����ܰ迡�� ���� ����
	KVector3(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat3(&m_Vector, _Other); }


public:
	static const KVector3 Zero;
	static const KVector3 Left;
	static const KVector3 Right;
	static const KVector3 Up;
	static const KVector3 Down;
	static const KVector3 Forword;
	static const KVector3 Back;
	static const KVector3 One; // ��������������


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

	// �翬�� ������ ��������.����������������
	KVector3 cross(const KVector2& _Left, const KVector2& _Right) const
	{
		return DirectX::XMVector3Cross(_Left, _Right);
	}

	// �翬�� ������ ��������.����������������
	KVector3 cross(const KVector2& _Right) const
	{
		return DirectX::XMVector3Cross(*this, _Right);
	}


	// �翬�� ������ ��������.����������������
	KVector3 cross(const KVector3& _Right) const
	{
		KVector3 Temp = *this * KPI / 180;
		return DirectX::XMVector3Cross(Temp, _Right);
	}

	// 3���� ���� �Ÿ� ���� -- �������� ���� -> 3���� �� ��ǥ������ �Ÿ�
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
	// �̺κ��� ��� ����� ���͸� �����ϸ鼭 ���Դ� �����Ӥ���
	// �̸� ���� ����ü -> ������� �ٷ� �߰�--- ����ü�� ���� �ƶ�
	union
	{
		// ���̷�ƮX ���� Vector3������ �����ϴ� ���� ������
		// �ۿ��ϴ� ���ʹ� �� ����(x, y, z, w)�� ��ȯ�Ѵ�.
		// ���� �� ���͸� ����� ������ �ִ� ����
		DirectX::XMFLOAT4 m_Vector;
		KVector2	m_Vector2;
		KVector3	m_Vector3;

		float s[4];


		// �̸����� ����ü -> ������� �ٷ� �߰��ؼ� ���ڰ��� ���ٰ���
		// �̳�Ŭ���� ������ ����ü�� �ƴ�
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
	static const KVector4 One; // ��������������


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

	// ���Ϳ��� �׶���� �÷� 4���� ��Ʋ����� �� ���� ������ ���� �� �� �ְ� ���� �ƿ� ����ܰ迡�� ���� ����
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

		// �������� ������ �ֱ� ����
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

	// ��ġ��� - �밢 ���� ��Ī
	void TransPose()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return;
	}

	// ����Ʈ�� ��ȯ���ִ� ��� - �����
	const KMatrix& Inverse_Referance()
	{
		// ����� ��Ľ��� ��ȯ�Ѵ�.
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(*this);
		*this = DirectX::XMMatrixInverse(&det, *this);
		return *this;
	}

	// ���� ��ȯ���ִ� ��� - �����
	KMatrix Inverse_Value() const
	{
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(*this);
		return DirectX::XMMatrixInverse(&det, *this);
	}

	// ����Ʈ�� ��ȯ���ִ� ��� - ��ġ ���
	const KMatrix& TransPose_Referance()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return *this;
	}

	// ���� ��ȯ���ִ� ��� - ��ġ ���
	KMatrix TransPose_Value() const
	{
		return DirectX::XMMatrixTranspose(*this);
	}

	// ViewAt -> ���忡 �����ǥ�� �������� -> �ȿ��� ���⺤�͸� ����
	// ��ġ�� �ش� ��ġ�� ���� ���⺤�͸� ����
	void ViewAt_LH(const KVector4& _CamPos, const KVector4& _LookWorldPos, const KVector4& _Up)
	{
		*this = DirectX::XMMatrixLookAtLH(_CamPos, _LookWorldPos, _Up);
		return;
	}

	// ���⺤�͸� �ٷ� �־� ������
	void ViewTo_LH(const KVector4& _CamPos, const KVector4& _LookDirPos, const KVector4& _Up)
	{
		*this = DirectX::XMMatrixLookToLH(_CamPos, _LookDirPos, _Up);
		return;
	}


	// ���� - ����
	void Proj_Perspective(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width / _Height, _Near, _Far);
		return;
	}

	// ���� - ����
	void Proj_Orthographic(float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
		return;
	}

	// ��ġ �Ӽ� ���� O
	KVector4 Multi_Vector_O(const KVector4& _Value) const
	{
		return DirectX::XMVector3TransformCoord(_Value, *this);
	}

	// ��ġ �Ӽ� ���� X
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