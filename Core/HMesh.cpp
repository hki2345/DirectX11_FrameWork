#include "HMesh.h"
#include "HVAR.h"


HMesh::HMesh() : m_VB(nullptr), m_IB(nullptr)
{
}

HMesh::~HMesh()
{
	if (nullptr != m_VB)
	{
		m_VB->Release();
	}

	if (nullptr != m_IB)
	{
		m_IB->Release();
	}
}


bool HMesh::Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem,
	UINT _IdxCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem,
	DXGI_FORMAT	_IdxFm, D3D11_PRIMITIVE_TOPOLOGY _eDrawMode)
{
	if (false == CreateVtx(_iVtxCount, _iVtxSize, _eVtxUsage, _VtxMem))
	{
		return false;
	}

	if (false == CreateIdx(_IdxCount, _iIdxSize, _eIdxUsage, _IdxMem))
	{
		return false;
	}

	m_IdxFm = _IdxFm;
	m_eDrawMode = _eDrawMode;
	m_VtxSize = _iVtxSize;
	m_IdxCount = _IdxCount;


	return true;
}

bool HMesh::CreateVtx(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem) 
{
	D3D11_BUFFER_DESC tBD = D3D11_BUFFER_DESC();

	tBD.ByteWidth = _iVtxCount * _iVtxSize;
	tBD.Usage = _eVtxUsage;

	if (D3D11_USAGE_DYNAMIC == tBD.Usage)
	{
		tBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _VtxMem;

	if (S_OK != HVAR::PDevice()->CreateBuffer(&tBD, &tSub, &m_VB))
	{
		return false;
	}

	return true;
}

bool HMesh::CreateIdx(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem)
{
	D3D11_BUFFER_DESC tBD = D3D11_BUFFER_DESC();

	tBD.ByteWidth = _iTriCount * _iIdxSize;
	tBD.Usage = _eIdxUsage;

	if (D3D11_USAGE_DYNAMIC == tBD.Usage)
	{
		tBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _IdxMem;

	if (S_OK != HVAR::PDevice()->CreateBuffer(&tBD, &tSub, &m_IB))
	{
		return false;
	}

	return true;
}

void HMesh::Update()
{
	UINT iOff = 0;
	UINT iSize = m_VtxSize;

	HVAR::Context()->IASetVertexBuffers(0, 1, &m_VB, &iSize, &iOff);
	HVAR::Context()->IASetPrimitiveTopology(m_eDrawMode);
	HVAR::Context()->IASetIndexBuffer(m_IB, m_IdxFm, 0);
}
void HMesh::Render() {
	HVAR::Context()->DrawIndexed(m_IdxCount, 0, 0);
}