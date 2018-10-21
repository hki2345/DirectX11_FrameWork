#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include <vector>


class KMesh : public Resource
{
private:
	// �̷��� �޽��� ��Ÿ������ ���۵� �ڷ����� ���� ���ѹ��ȴ�. (KDevice ��� ���� ����)
	ID3D11Buffer*				m_VertBuf; // ���ؽ� �����Դϴ�.
	ID3D11Buffer*				m_InxBuf; // �ε��� ����
	DXGI_FORMAT					m_IdxFormat; // �ε��� ���� ����
	D3D11_PRIMITIVE_TOPOLOGY	m_eDrawMode; 
	UINT					    m_VtxSize;
	UINT					    m_IdxCount;

public:
	// ���� ��ΰ�? UINT _iVtxCount
	// ���ϳ��� ũ�Ⱑ ���ΰ�. _iVtxSize
	// ���� �뵵�� �����ΰ�? D3D11_USAGE _eVtxUsage 
	// -> ���� ĥ�� ������ -> �̹����� ���� ������ -> ���� �ݻ��ų�������� ���� ��
	// ���ؽ��� ���� ���� void* _VtxMem

	// �ε��� ���� ���� UINT _iIdxCount 
	// �ε��� ������ �뵵�� �����̳�? D3D11_USAGE _eIdxUsage
	// ���� ����
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
	KMesh();
	~KMesh();
};

