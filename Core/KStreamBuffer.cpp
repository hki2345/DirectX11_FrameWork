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


// 스트림 버퍼 업데이트 부분
// 스트림 버퍼라 해도 버퍼는 그냥 버퍼임 ㅇㅇ
void KStreamBuffer::Update()
{
	KUINT BuffOffset = 0;
	Core_Class::Context()->SOSetTargets(1, &pBuffer, &BuffOffset);
}

// 정점이 나왔다가 그걸 가지고 파티클 꾸미는 과정에서
// 한번의 스트림 리셋이 필요하ㅣ다.
void KStreamBuffer::Reset()
{
	KUINT BuffOffset = 0;
	ID3D11Buffer* tBuffer[1] = { nullptr, };
	Core_Class::Context()->SOSetTargets(1, tBuffer, &BuffOffset);
}