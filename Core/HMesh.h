#pragma once
#include "HRes.h"
#include "DHeader.h"
#include <vector>


class HMesh : public HRes
{
private:
	ID3D11Buffer*				m_VB; // ���ؽ� �����Դϴ�.
	ID3D11Buffer*				m_IB; // �ε��� ����
	DXGI_FORMAT					m_IdxFm; // �ε��� ���� ����
	D3D11_PRIMITIVE_TOPOLOGY	m_eDrawMode; 
	UINT					    m_VtxSize;
	UINT					    m_IdxCount;

public:
	// ���� ��ΰ�? UINT _iVtxCount
	// ���ϳ��� ũ�Ⱑ ���ΰ�. _iVtxSize
	// ���� �뵵�� �����ΰ�? D3D11_USAGE _eVtxUsage
	// ���ؽ��� ���� ���� void* _VtxMem
	// �ε��� ���� ���� UINT _iIdxCount 
	// �ε��� ������ �뵵�� �����̳�? D3D11_USAGE _eIdxUsage
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

