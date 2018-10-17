#pragma once

// #define _XM_NO_INTRINSICS_

#include <d3d11.h> // 메인헤더
#include <d3dcompiler.h>// 쉐이더용 디버깅헤더
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

class HMATHVAR
{
public:
	static const float DTOR;
	static const float RTOD;

public:
	// 0.0000001
	static bool EpSilonCheckf(float _Value) 
	{
		if (fabsf(_Value) < 0.0000001f)
		{
			return true;
		}

		return false;
	}

	static float EpSilonCutf(float& _Value)
	{
		if (fabsf(_Value) < 0.0000001f)
		{
			_Value = 0.0f;
		}

		return _Value;
	}

private:
	HMATHVAR() {}
	~HMATHVAR() {}
};

class BaseColor
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

};

class HIndex
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
	HIndex(int _X, int  _Y) : X(_X), Y(_Y) {}
	HIndex(__int64 _Value) : m_i64(_Value) {  }
	~HIndex() {}
};


class HVEC2
{
public:
	union
	{
		DirectX::XMFLOAT2 m_Vec;
		float s[2];
		struct
		{
			float x;
			float y;
		};

		struct
		{
			int ix;
			int iy;
		};
		__int64 i64;
	};

public:
	HVEC2() : m_Vec(0, 0) {}
	HVEC2(__int64 _Var) : i64(_Var) {}
	HVEC2(float _Var) : m_Vec(_Var, _Var) {}
	HVEC2(float _x, float _y) : m_Vec(_x, _y) {}
	HVEC2(int _x, int _y) : ix(_x), iy(_y) {}
	HVEC2(const HVEC2& _Other) : m_Vec(_Other.x, _Other.y) {}
	HVEC2(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat2(&m_Vec, _Other); }

public:
	operator __int64() const
	{
		return i64;
	}


	void Setting(float _x, float _y) {
		x = _x; y = _y;
	}

	float RatioX() 
	{
		if (0 > y)
		{
			return 0.0f;
		}

		return x / y;
	}

	float RatioY()
	{
		if (0 > x)
		{
			return 0.0f;
		}

		return y / x;
	}

public:
	HVEC2 ConvertIntToFloat() 
	{
		return HVEC2{ (float)ix, (float)iy };
	}

public:
	float HX() const {
		return x * 0.5f;
	}

	float HY() const {
		return y * 0.5f;
	}

	HVEC2 operator -(const HVEC2& _Value) const
	{
		return{ x - _Value.x, y - _Value.y };
	}

	HVEC2 operator /(const HVEC2& _Value) const
	{
		if (0 >= _Value.x || 0 >= _Value.y)
		{
			return { 0.0f,0.0f };
		}

		return{ x / _Value.x, y / _Value.y };
	}

	HVEC2& operator -=(const HVEC2& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;
		return *this;
	}

	HVEC2 operator +(const HVEC2& _Value) const
	{
		return{x + _Value .x, y + _Value.y };
	}

	HVEC2 operator +=(const HVEC2& _Value)
	{
		x += _Value.x;
		y += _Value.y;
		return *this;
	}

	HVEC2& operator =(const HVEC2& _Value) 
	{
		m_Vec = _Value.m_Vec;
		return *this;
	}


	//HVEC2& operator =(const DirectX::XMVECTOR& _Value)
	//{
	//	DirectX::XMStoreFloat2(&m_Vec, _Value);
	//	return *this;
	//}

	float Len()
	{
		return sqrtf(x * x + y * y);
	}
};

typedef HVEC2 HPOINT;

class HVEC3
{
public:
	union
	{
		DirectX::XMFLOAT3 m_Vec;
		float s[3];
		struct
		{
			HVEC2 m_Pos;
			float Radius;
		};

		struct
		{
			float x;
			float y;
			float z;
		};
	};

public:
	HVEC3() : m_Vec(0, 0, 0) {}
	HVEC3(float _Var) : m_Vec(_Var, _Var, _Var) {}
	HVEC3(float _x, float _y) : m_Vec(_x, _y, 0.0f) {}
	HVEC3(float _x, float _y, float _z) : m_Vec(_x, _y, _z) {}
	HVEC3(const HVEC2& _Other) : m_Vec(_Other.x, _Other.y, 0.0f) {}
	HVEC3(const HVEC3& _Other) : m_Vec(_Other.x, _Other.y, _Other.z) {}
	HVEC3(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat3(&m_Vec, _Other); }

public:
	void Setting(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}



public:
	float Diameter() const
	{
		return Radius * 2.0f;
	}
	float HX() {
		return x * 0.5f;
	}

	float HY() {
		return y * 0.5f;
	}

	float HZ() {
		return z * 0.5f;
	}

	HVEC3 operator +(const HVEC3& _Value)
	{
		return{ x + _Value.x, y + _Value.y,  z + _Value.z };
	}

	HVEC3& operator =(const HVEC3& _Value)
	{
		m_Vec = _Value.m_Vec;
		return *this;
	}


	HVEC3& operator =(const DirectX::XMVECTOR& _Value)
	{
		DirectX::XMStoreFloat3(&m_Vec, _Value);
		return *this;
	}

	float Len()
	{
		return sqrtf(x * x + y * y + z * z);
	}
};

typedef HVEC3 HCIRCLE;

// 2디라고 2차원 벡터를 사용하지 않는다. 
// 2차원 벡터
#define HRGBA(r,g,b,a)          ( (COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g))<<8)) | ((DWORD)(BYTE)(b))<<16 | ((DWORD)(BYTE)(a))<<24 )

class HVEC 
{
public:
	static const HVEC LEFT;
	static const HVEC RIGHT;
	static const HVEC UP;
	static const HVEC DOWN;
	static const HVEC FORWORD;
	static const HVEC BACK;
	static const HVEC RX;
	static const HVEC RY;
	static const HVEC RZ;
	static const HVEC MRX;
	static const HVEC MRY;
	static const HVEC MRZ;
	static const HVEC ZERO;
	static const HVEC ONE;

	static const HVEC BLACK;
	static const HVEC RED;
	static const HVEC WHITE;
	static const HVEC GREEN;
	static const HVEC BLUE;
	static const HVEC MAGENTA;

public:
	union 
	{
		HVEC2			  m_Vec2;
		HVEC3			  m_Vec3;
		DirectX::XMFLOAT4 m_Vec;
		float s[4];
		struct 
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			HVEC2 m_Pos;
			HVEC2 m_Size;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		struct
		{
			int ir;
			int ig;
			int ib;
			int ia;
		};

		struct
		{
			unsigned int uir;
			unsigned int uig;
			unsigned int uib;
			unsigned int uia;
		};
	};

public:
	void SetColor(
		unsigned int _uir,
		unsigned int _uig,
		unsigned int _uib,
		unsigned int _uia
	) 
	{
		uir = _uir;
		uig	= _uig;
		uib	= _uib;
		uia	= _uia;
	}

public:
	HVEC() : m_Vec(0, 0, 0, 0) {}
	HVEC(float _Var) : m_Vec(_Var, _Var, _Var, _Var) {}
	HVEC(float _x, float _y) : m_Vec(_x, _y, 0.0f, 0.0f) {}
	HVEC(float _x, float _y, float _z) : m_Vec(_x, _y, _z, 0.0f) {}
	HVEC(float _x, float _y, float _z, float _w) : m_Vec(_x, _y, _z, _w) {}
	HVEC(const HVEC& _Other) : m_Vec(_Other.x, _Other.y, _Other.z, _Other.w) {}
	HVEC(const HVEC2& _Other) : m_Vec(_Other.x, _Other.y, 0.0f, 0.0f) {}
	HVEC(const HVEC2& _Pos, const HVEC2& _Size) : m_Pos(_Pos), m_Size(_Size) {}
	HVEC(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat4(&m_Vec, _Other); }

public: // Rect

	void EpCut() 
	{
		HMATHVAR::EpSilonCutf(x);
		HMATHVAR::EpSilonCutf(y);
		HMATHVAR::EpSilonCutf(z);
	}

	float HX() const {
		return x * 0.5f;
	}

	float HY() const {
		return y * 0.5f;
	}

	float HZ() const {
		return z * 0.5f;
	}

	float HW() const {
		return w * 0.5f;
	}

	float FLEFT() const {
		return m_Pos.x - m_Size.HX();
	}

	float FRIGHT() const {
		return m_Pos.x + m_Size.HX();
	}

	float FTOP() const {
		return m_Pos.y + m_Size.HY();
	}

	float FBOT() const {
		return m_Pos.y - m_Size.HY();
	}

	HVEC2 LT() const {
		return{ FLEFT(), FTOP() };
	}

	HVEC2 LB() const {
		return{ FLEFT(), FBOT() };
	}

	HVEC2 RT() const {
		return{ FRIGHT(), FTOP() };
	}

	HVEC2 RB() const {
		return{ FRIGHT(), FBOT() };
	}

	UINT Color255Convert() {
		return HRGBA((UINT)(r * 255), (UINT)(g * 255), (UINT)(b * 255), (UINT)(a * 255)));
	}


public:
	HVEC operator + (const HVEC& _Value) const
	{
		return HVEC(x + _Value.x, y + _Value.y, z + _Value.z, w + _Value.w);
	}

	HVEC operator - (const HVEC& _Value) const
	{
		return HVEC(x - _Value.x, y - _Value.y, z - _Value.z, w - _Value.w);
	}

	HVEC operator / (const HVEC& _Value) const
	{
		HVEC Vec;
		if (0 != _Value.x) 		{ 			Vec.x = x / _Value.x;		}		else {			Vec.x = 0.0f;		}
		if (0 != _Value.y)		{			Vec.y = y / _Value.y;		}		else {			Vec.y = 0.0f;		}
		if (0 != _Value.z)		{			Vec.z = z / _Value.z;		}		else {			Vec.z = 0.0f;		}
		if (0 != _Value.w)		{			Vec.w = w / _Value.w;		}		else {			Vec.w = 0.0f;		}

		return Vec;
	}

	HVEC& operator +=(const HVEC& _Value)
	{
		x += _Value.x; y += _Value.y; z += _Value.z; w += _Value.w;
		return *this;
	}

	HVEC operator * (const float _Value) const
	{
		return HVEC(x * _Value, y * _Value, z * _Value, w * _Value);
	}

	HVEC& operator *= (const float _Value)
	{
		x *= _Value, y *= _Value, z *= _Value, w *= _Value;
		return *this;
	}

	HVEC& operator =(const HVEC& _Value)
	{
		m_Vec = _Value.m_Vec;
		return *this;
	}


	HVEC& operator =(const DirectX::XMVECTOR& _Value)
	{
		DirectX::XMStoreFloat4(&m_Vec, _Value);
		return *this;
	}


	HVEC operator -()
	{
		return HVEC(-x , -y , -z , -w );
	}

	operator HVEC2() const
	{
		return HVEC2(m_Vec2);
	}

	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&m_Vec);
	}

public:
	static HVEC Cross3D(const HVEC& _Left, const HVEC& _Right)
	{
		HVEC Value = DirectX::XMVector3Cross(_Left, _Right);
		return Value;
	}
	HVEC Cross3D(const HVEC& _Other)
	{
		HVEC Value = DirectX::XMVector3Cross(*this, _Other);
		return Value;
	}

	static HVEC Dot(const HVEC& _Left, const HVEC& _Right)
	{
		HVEC Value = DirectX::XMVector4Dot(_Left, _Right);
		return Value.x;
	}
	float Dot(const HVEC& _Other)
	{
		HVEC Value = DirectX::XMVector4Dot(*this, _Other);
		return Value.x;
	}
	void Normalize4D()
	{
		*this = DirectX::XMVector4Normalize(*this);
	}

	void Normalize3D() 
	{
		*this = DirectX::XMVector3Normalize(*this);
		w = 0.0f;
	}

	void Normalize2D()
	{
		*this = DirectX::XMVector2Normalize(*this);
		z = 0.0f;
		w = 0.0f;
	}

	float Len()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	// 위치속성이 적용되지 않는다.
	HVEC ZRotToDir(float _Angle)
	{
		_Angle *= HMATHVAR::DTOR;

		x = cosf(_Angle);
		y = sinf(_Angle);

		HMATHVAR::EpSilonCutf(x);
		HMATHVAR::EpSilonCutf(y);

		return *this;
	}


};

typedef HVEC HCOLOR;
typedef HVEC HRECT;


class HMAT
{
public:
	union
	{
		DirectX::XMFLOAT4X4 m_Mat;

		struct 
		{
			HVEC v1;
			HVEC v2;
			HVEC v3;
			HVEC v4;
		};

		float m[4][4];
	};

public:
	HMAT() 
	{
	}

	HMAT(const HMAT& _Other)
	{
		memcpy_s(this, sizeof(HMAT),  &_Other, sizeof(HMAT));
	}

	HMAT(const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&m_Mat, _Other);
	}

public:
	operator DirectX::XMMATRIX() const
	{
		return DirectX::XMLoadFloat4x4(&m_Mat);
	}

	HMAT& operator=(const HMAT& _Other)
	{
		memcpy_s(this, sizeof(HMAT), &_Other, sizeof(HMAT));
		return *this;
	}


	HMAT& operator=(const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&m_Mat, _Other);
		return *this;
	}

	HMAT operator*(const DirectX::XMMATRIX& _Other) const
	{
		return DirectX::XMMatrixMultiply(*this, _Other);
	}

public:
	HMAT& Iden() 
	{
		*this = DirectX::XMMatrixIdentity();
		return *this;
	}

	void RotX(float _Value)
	{
		*this = DirectX::XMMatrixRotationX(_Value);
		return;
	}

	void RotY(float _Value)
	{
		*this = DirectX::XMMatrixRotationY(_Value);
		return;
	}

	void RotZ(float _Value)
	{
		*this = DirectX::XMMatrixRotationZ(_Value);
		return;
	}

	void Scale(const HVEC& _Scale)
	{
		*this = DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
		return;
	}

	void Trans(const HVEC& _Pos)
	{
		*this = DirectX::XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
		return;
	}

	void Transpose()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return;
	}

	const HMAT& RTranspose()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return *this;
	}

	HMAT RVTranspose() const
	{
		HMAT Mat = *this;
		Mat = DirectX::XMMatrixTranspose(Mat);
		return Mat;
	}

	void ViewAtLH(const HVEC& _EyePos, const HVEC& _LookPos, const HVEC& _UpDir)
	{
		*this = DirectX::XMMatrixLookAtLH(_EyePos, _LookPos, _UpDir);
		return;
	}

	void ViewToLH(const HVEC& _EyePos, const HVEC& _LookDir, const HVEC& _UpDir)
	{
		*this = DirectX::XMMatrixLookToLH(_EyePos, _LookDir, _UpDir);
		return;
	}

	// 원근투영
	// Near 근평면의 거리
	// _Far 원평면의 거리ㅏ
	void PersLH(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width/ _Height, _Near, _Far);
		return;
	}

	void OrthLH(float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
		return;
	}

	// 위치속성이 적용되고
	HVEC MulVecOne(const HVEC& _Vec) const
	{
		HMAT CalMat = *this;
		return DirectX::XMVector3TransformCoord(_Vec, CalMat);
	}

	// 위치속성이 적용되지 않는다.
	HVEC MulVecZero(const HVEC& _Vec) const
	{
		HMAT CalMat;
		memcpy_s(&CalMat, sizeof(HMAT), this, sizeof(HMAT));
		return DirectX::XMVector3TransformNormal(_Vec, CalMat);
	}
};

class DHelper
{
public:
	static UINT GetFmSize(DXGI_FORMAT _Fm);
};

class MATDATA 
{
public:
	HMAT m_W;
	HMAT m_V;
	HMAT m_P;
	HMAT m_WV;
	HMAT m_WVP;

public:
	void RTrans() {
		m_W.RTranspose();
		m_V.RTranspose();
		m_P.RTranspose();
		m_WV.RTranspose();
		m_WVP.RTranspose();
	}
};

// 3D그래픽 카드의 사용권한을 나눠달라고 구걸하는 애.
enum SHTYPE
{
	ST_NONE,
	ST_VS,
	ST_PS,
};

