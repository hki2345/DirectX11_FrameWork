#pragma once
#include "DirectXHeader.h"
#include "Mother_KWindow.h"
#include "SmartPtr.h"

#include <unordered_map>


// 본격적으로 그래픽카드의 권한 일부를 가져오는 놈이다.
// 여기서 이 구조는 어떻게 보면 Window, fmod에서 권한을 가져오는 것과 일맥상 통한다.
// 권한을 가져올 포인터 마련 -> 가져옴 -> 다른 연결된 포인터 가져옴 -> 적용
// 디바이스는 해당 권한을 불러오려는 윈도우를 알아야한다.
class KDevice : public Mother_KWindow
{
	// 렌더에서 해당 디바이스의 컨텍스트를 실행한다.
public:
	friend class Renderer;
	friend class RenderManager;

public:
	// 어떤식으로 보여지게 할 것이냐///
	// 앞면만 뒷면만
	// 모든면 와이어 -> 3D MAX 생각하면 됌
	enum RASTER_TPYE
	{
		RT_FRONT,
		RT_BACK,
		RT_ALL,
		RT_WIRE,
		RT_END,
	};

private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	ID3D11Texture2D*			m_pTexture2D;

	ID3D11Texture2D*			m_pBackBuffer;
	ID3D11RenderTargetView*		m_pTargetView;
	ID3D11DepthStencilView*		m_pDepthStencilView;

	D3D11_DEPTH_STENCIL_DESC	m_DepthDESC;
	ID3D11DepthStencilState*	m_pDepthStencilState;

	ID3D11DepthStencilState*	m_pDepthStencilStateDeg;

	IDXGISwapChain*				m_pSwapChain;
	UINT						m_iMSLv;
	KColor						m_Color;
	bool						m_bInit;

	// NUMCOLORS 컬러는 나중에 하는 걸로 ㅇㅇ -> 요것들을 렌더러에서 관리하는 것으로 변경
	//ID3D11RasterizerState*		m_ArrRs[(UINT)RASTER_TPYE::RT_END];

private:
	// 레스터라이즈 상태 -> 와이어 프레임인지 솔리드인지 결정은
	// 렌더러가 할 수 있게 한다.
	class RasterState : public SmartPtr
	{
	public:
		ID3D11DeviceContext* m_Context;
		D3D11_RASTERIZER_DESC	m_Desc;
		ID3D11RasterizerState* m_RS;

	public:
		void Update();
		void Create(ID3D11Device* _Device, ID3D11DeviceContext* _Context
			, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull);

	public:
		~RasterState()
		{
			if (nullptr != m_RS)
			{
				m_RS->Release();
			}
		}
	};

private:
	bool m_AllDefaultStateSetting;
	KPtr<RasterState> m_DefaultState;
	std::unordered_map<std::wstring, KPtr<RasterState>> m_RasterMap;

private:
	KPtr<RasterState> Find_RasterMode(const wchar_t* _Name);

public:
	void ResetRasterState();
	void Create_RasterMode(const wchar_t* _Name);
	void Create_RasterMode(const wchar_t* _Name, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull);

public:
	// 다이렉트 초기화를 여러번 해야 하는 경우가 생긴다. -> 초기화가 됐는데
	// 전체화면 도중 튕기거나 alt +tab을 눌러 바탕화면으로 빠져나올 경우에도
	// 디바이스를 잃어버릴 수도 있기 때문에 이러한 설정을 한다.
	bool& Is_Init()  { return m_bInit; }
	bool Init();

	ID3D11Device*				device() { return m_pDevice; }
	ID3D11DeviceContext*		context() { return m_pContext; }

	void reset_context();
	void SetOM();
	void SetOM_Deg();

public:
	bool Init_DefaultData_2D();
	bool Init_DefaultData_3D();

private:
	bool Create_SwapChain();
	bool Create_View();
	bool Create_ViewPort();

	bool DefaultRenderTarget();

	/********** 2D ************/
	// Sprite & Circle
	void Init_Sprite();
	void Init_Circle();



	/********** 3D ************/
	// Grid & Rect

	// Mesh
	void Init_RectMesh();
	void Init_CubeMesh();
	void Init_SphereMesh();

	// Material
	void Init_Defferd();
	void Init_NoneMat();
	void Init_ImageMat();
	void Init_ColorMat();

	void Init_GridMat();
	void Init_MeshMat();
	void Init_LightMat();



	void Clear_Target();
	void Present();
	void Release();




	/********** Const Buffer ************/

	class Device_CB : public SmartPtr
	{
	public:
		D3D11_BUFFER_DESC	tDesc;
		ID3D11Buffer*		p_ConstBuffer; // 요놈이 상수 버퍼 -> D11에선 쉐이더에서 쓰일 버퍼따위도
										   // 모두 요놈 자료형으로 저장된당
		UINT				iReg;

		~Device_CB()
		{
			if (nullptr != p_ConstBuffer) { p_ConstBuffer->Release(); }
		}
	};

private:
	std::unordered_map<std::wstring, KPtr<Device_CB>> m_CB_Map;

public:
	template<typename BUFTYPE>
	bool Create_DeviceCB(const wchar_t* _Name, D3D11_USAGE _eUsage, UINT _iReg)
	{
		Device_CB* NewBuf = new Device_CB();
		
		NewBuf->tDesc.ByteWidth = sizeof(BUFTYPE);
		NewBuf->tDesc.Usage = _eUsage;
		NewBuf->tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (NewBuf->tDesc.Usage == D3D11_USAGE_DYNAMIC)
		{
			NewBuf->tDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		}

		if (false == Create_DeviceCB(NewBuf))
		{
			return false;
		}

		NewBuf->iReg = _iReg;

		m_CB_Map.insert(std::unordered_map<std::wstring, KPtr<Device_CB>>::value_type(_Name, NewBuf));
		
		return true;
	}

	template<typename BUFTYPE>
	void Set_DeviceCB(const wchar_t* _Name, const BUFTYPE& _Data, SHADER_TYPE _Type)
	{
		KPtr<Device_CB> Buf = Find_DeviceCB(_Name);

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

		switch (_Type)
		{
		case ST_VS:
			m_pContext->VSSetConstantBuffers(Buf->iReg, 1, &Buf->p_ConstBuffer);
			break;
		case ST_PS:
			m_pContext->PSSetConstantBuffers(Buf->iReg, 1, &Buf->p_ConstBuffer);
			break;
		case ST_NONE:
		default:
			break;
		}
	}


public:
	bool Create_DeviceCB(Device_CB* _NewBuf);
	KPtr<Device_CB> Find_DeviceCB(const wchar_t* _Name);

public:
	KDevice(KWindow* _Window);
	~KDevice();
};

