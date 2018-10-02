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

	// ���̴� �ڵ带 ������ ������ ������ "�ڵ�"�� ����Ʈ ���
	// ���̴� �ڵ带 ������ ������ ������ "����"�� ����Ʈ ���
	ID3DBlob* m_Blob;
	ID3DBlob* m_Blob_Error;

	// �и���Ű�� �۾� ->��? ����̽��� �ٸ� �͵���
	ID3D11DeviceContext*	m_pContext;


	/******************************** ��� ���� ************************************/
public:
	class Const_Buffer : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC	tDesc;
		ID3D11Buffer*		p_ConstBuffer; // ����� ��� ���� -> D11���� ���̴����� ���� ���۵�����
										   // ��� ��� �ڷ������� ����ȴ�
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

		// ū �����͸� �ѹ��� �ѱ���� �ϸ� ������.
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
