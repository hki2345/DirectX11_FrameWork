#pragma once
#include "DirectXHeader.h"
#include "Mother_KWindow.h"
#include "SmartPtr.h"

#include <unordered_map>


// ���������� �׷���ī���� ���� �Ϻθ� �������� ���̴�.
// ���⼭ �� ������ ��� ���� Window, fmod���� ������ �������� �Ͱ� �ϸƻ� ���Ѵ�.
// ������ ������ ������ ���� -> ������ -> �ٸ� ����� ������ ������ -> ����
// ����̽��� �ش� ������ �ҷ������� �����츦 �˾ƾ��Ѵ�.
class KDevice : public Mother_KWindow
{
	// �������� �ش� ����̽��� ���ؽ�Ʈ�� �����Ѵ�.
public:
	friend class Renderer;
	friend class RenderManager;

public:
	// ������� �������� �� ���̳�///
	// �ո鸸 �޸鸸
	// ���� ���̾� -> 3D MAX �����ϸ� ��
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

	// NUMCOLORS �÷��� ���߿� �ϴ� �ɷ� ���� -> ��͵��� ���������� �����ϴ� ������ ����
	//ID3D11RasterizerState*		m_ArrRs[(UINT)RASTER_TPYE::RT_END];

private:
	// �����Ͷ����� ���� -> ���̾� ���������� �ָ������� ������
	// �������� �� �� �ְ� �Ѵ�.
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
	// ���̷�Ʈ �ʱ�ȭ�� ������ �ؾ� �ϴ� ��찡 �����. -> �ʱ�ȭ�� �ƴµ�
	// ��üȭ�� ���� ƨ��ų� alt +tab�� ���� ����ȭ������ �������� ��쿡��
	// ����̽��� �Ҿ���� ���� �ֱ� ������ �̷��� ������ �Ѵ�.
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
		ID3D11Buffer*		p_ConstBuffer; // ����� ��� ���� -> D11���� ���̴����� ���� ���۵�����
										   // ��� ��� �ڷ������� ����ȴ�
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

		// ū �����͸� �ѹ��� �ѱ���� �ϸ� ������.
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

