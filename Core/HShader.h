#pragma once
#include "HRes.h"
#include "DHeader.h"
#include <unordered_map>

class HShader : public HRes
{
public:

protected:
	SHTYPE      m_Type;
	ID3DBlob*	m_pBlob; // 쉐이더 코드를 컴파일했을때 나오는 코드의 바이트 배열
	ID3DBlob*	m_pErrBlob; // 쉐이더 코드를 컴파일했을때 나오는 에러의 바이트 배열
	ID3D11DeviceContext*		m_pContext;				// 컨텍스트

public:
	SHTYPE ShaderType() {
		return m_Type;
	}

public:
	void DefInit();


protected:
	// 윈도우 기반이기 때문에 Window가 그냥 값으로 들고 있을 것이다.
	class CBUFFER : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC tDesc;
		ID3D11Buffer*	pConstBuffer;
		UINT			iReg;

		~CBUFFER()
		{
			if (nullptr != pConstBuffer) { pConstBuffer->Release(); };
		}
	};

private:
	std::unordered_map<std::wstring, KPtr<CBUFFER>> m_MapConstBuffer;

public:
	template<typename BUFTYPE>
	bool CreateCB(const wchar_t* _Name, D3D11_USAGE _eUsage, UINT _iReg)
	{
		CBUFFER* NewBuf = new CBUFFER();

		NewBuf->tDesc.ByteWidth = sizeof(BUFTYPE);
		NewBuf->tDesc.Usage = _eUsage;
		NewBuf->tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (NewBuf->tDesc.Usage == D3D11_USAGE_DYNAMIC)
		{
			NewBuf->tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (false == CreateCB(NewBuf))
		{
			return false;
		}

		NewBuf->iReg = _iReg;

		m_MapConstBuffer.insert(std::unordered_map<std::wstring, KPtr<CBUFFER>>::value_type(_Name, NewBuf));

		return true;
	}

	template<typename BUFTYPE>
	void SettingCB(const wchar_t* _Name, const BUFTYPE& _Data)
	{
		KPtr<CBUFFER> Buf = FindCB(_Name);

		if (nullptr == Buf || Buf->tDesc.ByteWidth != sizeof(BUFTYPE))
		{
			return;
		}

		// 큰 데이터를 한번에 넘기려고 하면 느리다.
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		m_pContext->Map(Buf->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, &_Data, sizeof(BUFTYPE));
		m_pContext->Unmap(Buf->pConstBuffer, 0);

		UpdateCB(Buf);
	}

	bool CreateCB(CBUFFER* NewBuf);
	KPtr<CBUFFER> FindCB(const wchar_t* _Name);


private:
	virtual void Update() = 0;
	virtual void UpdateCB(KPtr<CBUFFER> Buf) = 0;

public:
	HShader();
	~HShader();
};

