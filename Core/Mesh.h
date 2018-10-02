#pragma once
#include "KResource.h"
#include "DirectXHeader.h"

struct Vertex
{
	KVector4 m_Pos;
	KVector4 m_Uv;
};

struct VTexColor
{
	KVector4 m_Pos;
	KVector4 m_Color;
};

class Mesh : public KResource
{
private:
	// 이렇게 메쉬를 나타나내는 버퍼도 자료형을 통일 시켜버렸다. (KDevice 상수 버퍼 참고)
	ID3D11Buffer*				m_VertexBuffer; // 버텍스 버퍼
	ID3D11Buffer*				m_IndexBuffer;	// 인덱스 버퍼
	DXGI_FORMAT					m_IndexFormat;	// 인덱스 버퍼 포멧
	D3D11_PRIMITIVE_TOPOLOGY	m_eDrawMode;

	UINT						m_VertexSize;
	UINT						m_InX_Cnt;

public:
	// 점이 몇개인가? UINT _iVtxCount
	// 점하나의 크기가 얼마인가. _iVtxSize
	// 점의 용도는 무엇인가? D3D11_USAGE _eVtxUsage 
	// -> 색을 칠할 것인지 -> 이미지를 입힐 것인지 -> 빛을 반사시킬것인지에 대한 값
	// 버텍스의 실제 내용 void* _VtxMem
	
	// 인덱스 버퍼 개수 UINT _iIdxCount 
	// 인덱스 버퍼의 용도가 무엇이냐? D3D11_USAGE _eIdxUsage
	// 이하 동문
	bool Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem,
		UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem, DXGI_FORMAT	_IdxFm
		, D3D11_PRIMITIVE_TOPOLOGY	_eDrawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	bool Create_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem);
	bool Create_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem);

public:
	void Update();
	void Render();

public:
	Mesh();
	~Mesh();
};

