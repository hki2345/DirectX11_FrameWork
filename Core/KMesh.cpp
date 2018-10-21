#include "KMesh.h"
#include "Core_Class.h"


KMesh::KMesh() : m_VertBuf(nullptr), m_InxBuf(nullptr)
{
}

KMesh::~KMesh()
{
	if (nullptr != m_VertBuf)
	{
		m_VertBuf->Release();
	}

	if (nullptr != m_InxBuf)
	{
		m_InxBuf->Release();
	}
}


bool KMesh::Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem,
	UINT _IdxCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem,
	DXGI_FORMAT	_IdxFm, D3D11_PRIMITIVE_TOPOLOGY _eDrawMode)
{
	if (false == Create_Vertex(_iVtxCount, _iVtxSize, _eVtxUsage, _VtxMem))
	{
		return false;
	}

	if (false == Create_Index(_IdxCount, _iIdxSize, _eIdxUsage, _IdxMem))
	{
		return false;
	}

	m_IdxFormat = _IdxFm;
	m_eDrawMode = _eDrawMode;
	m_VtxSize = _iVtxSize;
	m_IdxCount = _IdxCount;


	return true;
}

bool KMesh::Create_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem) 
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

	if (S_OK != Core_Class::PDevice()->CreateBuffer(&tBD, &tSub, &m_VertBuf))
	{
		return false;
	}

	return true;
}

bool KMesh::Create_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem)
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

	if (S_OK != Core_Class::PDevice()->CreateBuffer(&tBD, &tSub, &m_InxBuf))
	{
		return false;
	}

	return true;
}

void KMesh::Update()
{
	UINT iOff = 0;
	UINT iSize = m_VtxSize;

	Core_Class::Context()->IASetVertexBuffers(0, 1, &m_VertBuf, &iSize, &iOff);
	Core_Class::Context()->IASetPrimitiveTopology(m_eDrawMode);
	Core_Class::Context()->IASetIndexBuffer(m_InxBuf, m_IdxFormat, 0);
}
void KMesh::Render() {
	Core_Class::Context()->DrawIndexed(m_IdxCount, 0, 0);
}