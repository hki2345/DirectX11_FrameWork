#pragma once
#include "Resource.h"
#include "DXContainer.h"



// ��ƼŬ ���̴��� ��Ʈ�� �������� ������ ���ؤ� �������
class KStreamBuffer : public Resource
{
private:
	D3D11_BUFFER_DESC	tDesc;
	ID3D11Buffer*		pBuffer;
	UINT				iReg;


public:
	ID3D11Buffer* buffer()
	{
		return pBuffer;
	}

public:
	bool Create(const KUINT _BufSize, const KUINT& _Flag);
	void Update();
	void Reset();

public:
	KStreamBuffer();
	~KStreamBuffer();
};

