#pragma once
#include "HRes.h"
#include "DHeader.h"
#include <vector>


class HMesh : public HRes
{
private:
	ID3D11Buffer*				m_VB; // 버텍스 버퍼입니다.
	ID3D11Buffer*				m_IB; // 인덱스 버퍼
	DXGI_FORMAT					m_IdxFm; // 인덱스 버퍼 포멧
	D3D11_PRIMITIVE_TOPOLOGY	m_eDrawMode; 
	UINT					    m_VtxSize;
	UINT					    m_IdxCount;

public:
	// 점이 몇개인가? UINT _iVtxCount
	// 점하나의 크기가 얼마인가. _iVtxSize
	// 점의 용도는 무엇인가? D3D11_USAGE _eVtxUsage
	// 버텍스의 실제 내용 void* _VtxMem
	// 인덱스 버퍼 개수 UINT _iIdxCount 
	// 인덱스 버퍼의 용도가 무엇이냐? D3D11_USAGE _eIdxUsage
	bool Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem, 
		UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem, DXGI_FORMAT	_IdxFm
	, D3D11_PRIMITIVE_TOPOLOGY	_eDrawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	bool CreateVtx(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem);
	bool CreateIdx(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem);

public:
	void Update();
	void Render();

public:
	HMesh();
	~HMesh();
};

