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
	// 마소의 프로그래밍 방식중 하나.
	// 컴객체 방식에 대해서 공부해 보세요.
	// 리소스 관련 내용과 함수들에 대한 객체
	ID3D11Device*				m_pDevice;				// 최종적으로 디바이스의 장치 권한을 얻어오는 것
	// 랜더링 관련
	ID3D11DeviceContext*		m_pContext;				// 컨텍스트

	// 스왑체인에서 빠져나온 텍스처로 만든 타겟은 최종 랜더타겟으로 이용된다.
	IDXGISwapChain*				m_pSwapChain;			// 출력을 위한 스왑체인을 정의하기 위한 객체

	ID3D11Texture2D*			m_pBackBuffer;		// 깊이 스텐실용 텍스처
	ID3D11RenderTargetView*		m_pTagetView;			// 출력될 타겟 뷰

	ID3D11Texture2D*			m_pDepthStencilTex;		// 깊이 스텐실용 텍스처
	ID3D11DepthStencilView*		m_pDepthStencilView;	// 깊이 스텐실 뷰

	//D3D11_DEPTH_STENCIL_DESC	m_DepthState;
	//ID3D11DepthStencilState*	m_pDepthStencilState;	// 깊이 스텐실 뷰
	//ID3D11DepthStencilState*	m_pDepthStencilStateDebug;	// 깊이 스텐실 뷰


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
		// 윈도우 기반이기 때문에 Window가 그냥 값으로 들고 있을 것이다.
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

		// 큰 데이터를 한번에 넘기려고 하면 느리다.
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

