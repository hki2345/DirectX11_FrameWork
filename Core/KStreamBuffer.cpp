#include "KStreamBuffer.h"
#include "Core_Class.h"



KStreamBuffer::KStreamBuffer()
{
}


KStreamBuffer::~KStreamBuffer()
{
	if (nullptr != pBuffer)
	{
		pBuffer->Release();
	}
}


bool KStreamBuffer::Create(const KUINT _BufSize, const KUINT& _Flag)
{
	tDesc.ByteWidth = _BufSize;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;
	tDesc.BindFlags = _Flag;


	if (S_OK != Core_Class::PDevice()->CreateBuffer(&tDesc, nullptr, &pBuffer))
	{
		BBY;
		return false;
	}

	return true;
}


// ��Ʈ�� ���� ������Ʈ �κ�
// ��Ʈ�� ���۶� �ص� ���۴� �׳� ������ ����
void KStreamBuffer::Update()
{
	KUINT BuffOffset = 0;
	Core_Class::Context()->SOSetTargets(1, &pBuffer, &BuffOffset);
}

// ������ ���Դٰ� �װ� ������ ��ƼŬ �ٹ̴� ��������
// �ѹ��� ��Ʈ�� ������ �ʿ��ϤӴ�.
void KStreamBuffer::Reset()
{
	KUINT BuffOffset = 0;
	ID3D11Buffer* tBuffer[1] = { nullptr, };
	Core_Class::Context()->SOSetTargets(1, tBuffer, &BuffOffset);
}