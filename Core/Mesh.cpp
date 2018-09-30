#include "Mesh.h"
#include "Core_Class.h"



Mesh::Mesh() : m_VertexBuffer(nullptr), m_IndexBuffer(nullptr)
{
}


Mesh::~Mesh()
{
	if (nullptr != m_VertexBuffer)
	{
		m_VertexBuffer->Release();
	}

	if (nullptr != m_IndexBuffer)
	{
		m_IndexBuffer->Release();
	}
}


bool Mesh::Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem,
	UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem,
	DXGI_FORMAT	_IdxFm, D3D11_PRIMITIVE_TOPOLOGY _eDrawMode)
{
	if (false == Create_Vertex(_iVtxCount, _iVtxSize, _eVtxUsage, _VtxMem))
	{
		return false;
	}

	if (false == Create_Index(_iTriCount, _iIdxSize, _eIdxUsage, _IdxMem))
	{
		return false;
	}

	m_IndexFormat = _IdxFm;
	m_eDrawMode = _eDrawMode;
	m_VertexSize = _iVtxSize;
	m_InX_Cnt = _iTriCount;

	return true;
}

bool Mesh::Create_Vertex(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage, void* _VtxMem)
{
	// 버퍼를 입력하는 것은 역시 
	// 이전에 초기화라든지 뭐 그런 거 ... 여기서는 인자값을 넣어서 한다.
	D3D11_BUFFER_DESC tBD = D3D11_BUFFER_DESC();

	// 크기는 개수 * 하나의 크기
	tBD.ByteWidth = _iVtxCount * _iVtxSize;
	tBD.Usage = _eVtxUsage;

	// 
	if (D3D11_USAGE_DYNAMIC == tBD.Usage)
	{
		tBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = D3D11_SUBRESOURCE_DATA();
	tSub.pSysMem = _VtxMem;

	if (S_OK != Core_Class::device()->CreateBuffer(&tBD, &tSub, &m_VertexBuffer))
	{
		return false;
	}

	return true;
}

bool Mesh::Create_Index(UINT _iTriCount, UINT _iIdxSize, D3D11_USAGE _eIdxUsage, void* _IdxMem)
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

	if (S_OK != Core_Class::device()->CreateBuffer(&tBD, &tSub, &m_IndexBuffer))
	{
		return false;
	}

	return true;
}

void Mesh::Update()
{
	UINT iOff = 0;
	UINT iSize = m_VertexSize;

	Core_Class::device_context()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &iSize, &iOff);
	Core_Class::device_context()->IASetPrimitiveTopology(m_eDrawMode);
	Core_Class::device_context()->IASetIndexBuffer(m_IndexBuffer, m_IndexFormat, 0);
}

void Mesh::Render()
{
	Core_Class::device_context()->DrawIndexed(m_InX_Cnt, 0, 0);
}
