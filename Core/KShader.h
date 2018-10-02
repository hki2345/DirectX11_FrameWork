///
#pragma once
#include "KResource.h"
#include "DirectXHeader.h"
#include <unordered_map>

class KShader : public KResource
{
public:
	SHADER_TYPE& type() { return m_Type; }

protected:
	SHADER_TYPE m_Type;

	// 쉐이더 코드를 컴파일 했을때 나오는 "코드"의 바이트 행렬
	// 쉐이더 코드를 컴파일 했을때 나오는 "에러"의 바이트 행렬
	ID3DBlob* m_Blob;
	ID3DBlob* m_Blob_Error;

	// 분리시키는 작업 ->뭘? 디바이스와 다른 것들을
	ID3D11DeviceContext*	m_pContext;


	/******************************** 상수 버퍼 ************************************/
public:
	class Const_Buffer : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC	tDesc;
		ID3D11Buffer*		p_ConstBuffer; // 요놈이 상수 버퍼 -> D11에선 쉐이더에서 쓰일 버퍼따위도
										   // 모두 요놈 자료형으로 저장된당
		UINT				iReg;

		~Const_Buffer()
		{
			if (nullptr != p_ConstBuffer) { p_ConstBuffer->Release(); }
		}
	};

private:
	std::unordered_map<std::wstring, KPtr<Const_Buffer>> m_CB_Map;

public:
	template<typename BUFTYPE>
	bool Create_ConstBuf(const wchar_t* _Name, D3D11_USAGE _eUsage, UINT _iReg)
	{
		Const_Buffer* NewBuf = new Const_Buffer();

		NewBuf->tDesc.ByteWidth = sizeof(BUFTYPE);
		NewBuf->tDesc.Usage = _eUsage;
		NewBuf->tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (NewBuf->tDesc.Usage == D3D11_USAGE_DYNAMIC)
		{
			NewBuf->tDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		}

		if (false == Create_ConstBuf(NewBuf))
		{
			return false;
		}

		NewBuf->iReg = _iReg;

		m_CB_Map.insert(std::unordered_map<std::wstring, KPtr<Const_Buffer>>::value_type(_Name, NewBuf));
		return true;
	}

	template<typename BUFTYPE>
	void Set_ConstBuffer(const wchar_t* _Name, const BUFTYPE& _Data)
	{
		KPtr<Const_Buffer> Buf = Find_ConstBuffer(_Name);

		if (nullptr == Buf || Buf->tDesc.ByteWidth != sizeof(BUFTYPE))
		{
			KASSERT(true);
			return;
		}

		// 큰 데이터를 한번에 넘기려고 하면 느리다.
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		m_pContext->Map(Buf->p_ConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, &_Data, sizeof(BUFTYPE));
		m_pContext->Unmap(Buf->p_ConstBuffer, 0);

		UpdateCB(Buf);
	}


public:
	void Init_Default();
	bool Create_ConstBuf(Const_Buffer* _NewBuf);
	KPtr<Const_Buffer> Find_ConstBuffer(const wchar_t* _Name);


private:
	virtual void Update() = 0;
	virtual void UpdateCB(KPtr<Const_Buffer> _Buf) = 0;

public:
	KShader();
	~KShader();
};
