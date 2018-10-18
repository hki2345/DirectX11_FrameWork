#pragma once
#include "DHeader.h"
#include "HWindowBase.h"
#include "SmartPtr.h"
#include "KMacro.h"

#include <string>
#include <unordered_map>

class HDevice : public HWindowBase
{
public:
	friend class HRenderer;
	friend class HRenderMgr;

public:
	enum RS_TYPE
	{
		RT_FRONT,
		RT_BACK,
		RT_NONE,
		RT_WIREFRAME,
		END,
	};

private:
	// ������ ���α׷��� ����� �ϳ�.
	// �İ�ü ��Ŀ� ���ؼ� ������ ������.
	// ���ҽ� ���� ����� �Լ��鿡 ���� ��ü
	ID3D11Device*				m_pDevice;				// ���������� ����̽��� ��ġ ������ ������ ��
	// ������ ����
	ID3D11DeviceContext*		m_pContext;				// ���ؽ�Ʈ

	// ����ü�ο��� �������� �ؽ�ó�� ���� Ÿ���� ���� ����Ÿ������ �̿�ȴ�.
	IDXGISwapChain*				m_pSwapChain;			// ����� ���� ����ü���� �����ϱ� ���� ��ü

	ID3D11Texture2D*			m_pBackBuffer;		// ���� ���ٽǿ� �ؽ�ó
	ID3D11RenderTargetView*		m_pTagetView;			// ��µ� Ÿ�� ��

	ID3D11Texture2D*			m_pDepthStencilTex;		// ���� ���ٽǿ� �ؽ�ó
	ID3D11DepthStencilView*		m_pDepthStencilView;	// ���� ���ٽ� ��

	//D3D11_DEPTH_STENCIL_DESC	m_DepthState;
	//ID3D11DepthStencilState*	m_pDepthStencilState;	// ���� ���ٽ� ��
	//ID3D11DepthStencilState*	m_pDepthStencilStateDebug;	// ���� ���ٽ� ��


	UINT						m_iMSLv;
	HCOLOR						m_Color;
	bool						m_bInit;


public:
	ID3D11Device*				PDevice() { return m_pDevice; };
	ID3D11DeviceContext*		Context() { return m_pContext; };
	ID3D11DepthStencilView*		Depth() { return m_pDepthStencilView; };

private:
	class RSState : public SmartPtr
	{
	public:
		ID3D11DeviceContext* m_pContext;
		D3D11_RASTERIZER_DESC m_Desc;
		ID3D11RasterizerState* m_pRS;

	public:
		void Update();
		void Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode);

	public:
		~RSState() 
		{
			if (nullptr != m_pRS)
			{
				m_pRS->Release();
			}
		}
	};

private:
	KPtr<RSState> m_DefaultRState;
	std::unordered_map<std::wstring, KPtr<RSState>> m_RSMap;

private:
	KPtr<RSState> FindRsMode(const wchar_t* _Name);

public:
	void ResetRSState();
	void SetDefRsMode(const wchar_t* _Name);
	void CreateRsMode(const wchar_t* _Name, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode);
	void SetRsMode(const wchar_t* _Name);

private:
	class DSState : public SmartPtr
	{
	public:
		ID3D11DeviceContext* m_pContext;
		D3D11_DEPTH_STENCIL_DESC m_Desc;
		ID3D11DepthStencilState* m_pDS;

	public:
		void Update(unsigned int _Ref = 0);
		void Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_DEPTH_STENCIL_DESC _Desc);

	public:
		~DSState()
		{
			if (nullptr != m_pDS)
			{
				m_pDS->Release();
			}
		}
	};

private:
	KPtr<DSState> m_DefaultDState;
	std::unordered_map<std::wstring, KPtr<DSState>> m_DSMap;

private:
	KPtr<DSState> FindDsMode(const wchar_t* _Name);


public:
	void ResetDSState();
	void SetDefDsMode(const wchar_t* _Name);
	void CreateDsMode(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc);

	void SetDsMode(const wchar_t* _Name, unsigned int _Ref = 0);

public:
	void SetBsMode(const wchar_t* _Name);

public:
	bool CreateSwapChain();
	bool CreateView();
	bool CreateViewPort();
	bool DefaultDataInit();


	bool DefaultDataInit3D();
	bool DefRenderTaget();
	bool Def3DCreate();
	bool Mesh3DCreate();
	bool Mat3DCreate();
	

public:
	void ResetContext();
	void OMSet();

private:
	void ClearTaget();
	void Present();

public:
	bool Init();

	bool IsInit() {
		return m_bInit;
	}

public:
	void Release();

////////////////////////////////////////// ConstBuffer
private:
		// ������ ����̱� ������ Window�� �׳� ������ ��� ���� ���̴�.
		class GCBUFFER : public SmartPtr
		{
		public:
			D3D11_BUFFER_DESC tDesc;
			ID3D11Buffer*	pConstBuffer;
			UINT			iReg;

			~GCBUFFER()
			{
				if (nullptr != pConstBuffer) { pConstBuffer->Release(); };
			}
		};

private:
	std::unordered_map<std::wstring, KPtr<GCBUFFER>> m_MapConstBuffer;

public:
	template<typename BUFTYPE>
	bool CreateCB(const wchar_t* _Name, D3D11_USAGE _eUsage, UINT _iReg)
	{
		GCBUFFER* NewBuf = new GCBUFFER();

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

		m_MapConstBuffer.insert(std::unordered_map<std::wstring, KPtr<GCBUFFER>>::value_type(_Name, NewBuf));

		return true;
	}

	template<typename BUFTYPE>
	void SettingCB(const wchar_t* _Name, const BUFTYPE& _Data, SHTYPE _eType)
	{
		KPtr<GCBUFFER> Buf = FindCB(_Name);

		if (nullptr == Buf || Buf->tDesc.ByteWidth != sizeof(BUFTYPE))
		{
			return;
		}

		// ū �����͸� �ѹ��� �ѱ���� �ϸ� ������.
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		m_pContext->Map(Buf->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, &_Data, sizeof(BUFTYPE));
		m_pContext->Unmap(Buf->pConstBuffer, 0);

		switch (_eType)
		{
		case ST_VS:
			m_pContext->VSSetConstantBuffers(Buf->iReg, 1, &Buf->pConstBuffer);
			break;
		case ST_PS:
			m_pContext->PSSetConstantBuffers(Buf->iReg, 1, &Buf->pConstBuffer);
			break;
		default:
			break;
		}

	}

	bool CreateCB(GCBUFFER* NewBuf);
	KPtr<GCBUFFER> FindCB(const wchar_t* _Name);

public:
	HDevice(HWindow* _Win);
	~HDevice();
};

