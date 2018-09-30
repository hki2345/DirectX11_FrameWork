#pragma once
#include "DirectXHeader.h"


#define TypeS(TYPE) public: \
	static UINT TypeSize() { \
		return (UINT)sizeof(TYPE); \
	}

#define MemS(TYPE) public: \
	static UINT MemberSize() { \
		return (UINT)sizeof(TYPE); \
	}


class Vertex_3D
{
public:
	KVector Pos;
	KVector2 Uv;
	KVector Color;
	KVector Normal;

	TypeS(Vertex_3D)
};

class Vertex_Color
{
public:
	KVector4 Pos;
	KVector4 Color;

	TypeS(Vertex_Color)
};

class Vertex_Texture
{
public:
	KVector4 Pos;
	KVector2 Uv;

	TypeS(Vertex_Texture)
};


class KBorder
{
public:
	KColor		m_Color;
	KVector2	m_UvPerPixel;
	KVector2	m_Border;

	TypeS(KBorder)
};

// WORD 3°³
class Index_16
{
public:
	WORD _1, _2, _3;

	MemS(WORD)

		Index_16() : _1(0), _2(0), _3(0)
	{

	}

	Index_16(WORD __1, WORD __2, WORD __3) : _1(__1), _2(__2), _3(__3)
	{
	}

	static DXGI_FORMAT FM() { return DXGI_FORMAT_R16_UINT; }
};

// DWORD 3°³
class Index_32
{
public:
	DWORD _1, _2, _3;

	MemS(DWORD)

		Index_32() : _1(0), _2(0), _3(0)
	{

	}

	Index_32(DWORD __1, DWORD __2, DWORD __3) : _1(__1), _2(__2), _3(__3)
	{
	}

	static DXGI_FORMAT FM() { return DXGI_FORMAT_R32_UINT; }
};