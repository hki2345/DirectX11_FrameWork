#include "KDevice.h"
#include "KWindow.h"
#include "Core_Class.h"
#include "Pixel_Shader.h"
#include "Stl_Assistor.h"

#include "DirectXHeader_DE.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Blend.h"

#include "KMath.h"
#include "KFont.h"



KDevice::KDevice(KWindow* _Window) : Mother_KWindow(_Window),
m_pDevice(nullptr),
m_pContext(nullptr),
m_pTargetView(nullptr),
m_pTexture2D(nullptr),
m_pDS_View(nullptr),
m_pSwapChain(nullptr),
m_bInit(false),
m_Color(.5f, .5f, .5f, 1.0f)
{
}

KDevice::~KDevice()
{
	Release();
}

void KDevice::Release()
{
	if (nullptr != m_pDS_View) { m_pDS_View->Release(); }
	if (nullptr != m_pTargetView) { m_pTargetView->Release(); }
	if (nullptr != m_pSwapChain) { m_pSwapChain->Release(); }
	if (nullptr != m_pBackBuffer) { m_pBackBuffer->Release(); }
	if (nullptr != m_pTexture2D) { m_pTexture2D->Release(); }
	if (nullptr != m_pDevice) { m_pDevice->Release(); }
	if (nullptr != m_pContext) { m_pContext->Release(); }
}




bool KDevice::Init()
{
	m_bInit = false;
	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// � �������� ����̽��� ������ ���̳�-- �ٵ� �ڵ��������ִϱ� �� �װ� ����.
	// �뵵�� ���� ������ ������� ã�� ���̴�.
	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE ��� �ߴٸ� 3D �ܺ� 3D��ġ�� ã�´�. 
	// -> ��ī�� ã�´ٴ� ��
	// ����Ʈ�� ���� CPU�� ���� ó�� - ������ �ص� �̳����� �� ���� �ʳ� ��������

	//  
	if (S_OK != D3D11CreateDevice(
		/*Ư����Ȳ*/nullptr, /*�ܺ���ġ*/D3D_DRIVER_TYPE_HARDWARE
		, /*�ܺθ��*/nullptr, /*Flag*/ iFlag, 
		/*??*/nullptr,/*??*/ 0,/*SDK ����*/ D3D11_SDK_VERSION, 
		/*�޾ƿ� ��ġ*/&m_pDevice, /*�޾ƿ� ���� ����*/&eLv, 
		/*�޾ƿ� Context - ������ ���߿��Ѵ�µ� ���������� ��� ����*/&m_pContext))
	{
		Release();
		return false;
	}

	// ��Ƽ ���ø� ������ üũ�Ѵ�.
	// ��ܰ�� �ε巴�� �� ���̳� -> �� �ܰ�� Ȯ���Ų �̹����� �ռ��� ���̳�
	// ���⼭�� 4�ܰ�� �Ͽ���.
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iMSLv);

	// ��ȯ���� 0�̸� ���� �߻��� ���� - �翬 �ȵǾ�¡
	if (0 == m_iMSLv) { Release(); return false; }

	if (false == Create_SwapChain()) { Release(); return false; }
	if (false == Create_View()) { Release(); return false; }
	if (false == Create_ViewPort()) { Release(); return false; }
	

	m_bInit = true;
	return true;
}


bool KDevice::Create_SwapChain()
{
	// �� �� ���� ����ü��. -> �̰͵��� ��� �ʱ�ȭ���־�� �Ѵꤾ������
	// ���� �׳� �Ǵ� �� �ƴϰ�... ��� Ÿ�� Ÿ�� Ÿ�� ���� �ʱ�ȭ �ϴ� ��
	/*DXGI_MODE_DESC BufferDesc;
	DXGI_SAMPLE_DESC SampleDesc;
	DXGI_USAGE BufferUsage;
	UINT BufferCount;
	HWND OutputWindow;
	BOOL Windowed;
	DXGI_SWAP_EFFECT SwapEffect;
	UINT Flags;*/
	DXGI_SWAP_CHAIN_DESC m_Decs = {};

	// ����� ũ��
	m_Decs.BufferDesc.Width = (UINT)window()->width();
	m_Decs.BufferDesc.Height = (UINT)window()->heigth();

	// ����� �ֻ��� + ����� ����
	m_Decs.BufferDesc.RefreshRate.Numerator = 60;
	m_Decs.BufferDesc.RefreshRate.Denominator = 1;

	// ������ �Ȼ� - ��� �� �������� ������ �Ȼ��� ä��ڴ�.
	m_Decs.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ��ĵ ���� ������ ���� - �ָ� ����
	m_Decs.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_Decs.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ������ �뵵 - ȭ�� ����
	m_Decs.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;


	// ���ø� ���� - �̰� ���� �ܰ迡�� �� �ܰ�� �����ϴ��� �޾ƿ� ���̴�.
	m_Decs.SampleDesc.Count = m_iMSLv;
	m_Decs.SampleDesc.Quality = m_iMSLv - 1;


	// ������ ����
	m_Decs.OutputWindow = window()->KWnd();

	// �����Ҷ� ����Ʈ- �츰 �����.
	m_Decs.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// ���� ����
	m_Decs.BufferCount = 1;

	// ��üȭ�� ���� - �ش� �������� �켱���� __ ��üȭ���� �ֿ켱
	m_Decs.Windowed = !window()->Is_Full();


	/*********** ���� ���� *************/ // �İ�ü�� ������������
	IDXGIDevice* p_Device = nullptr;
	IDXGIAdapter* p_Adapter = nullptr;
	IDXGIFactory* p_Factory = nullptr;

	m_pDevice->QueryInterface(__uuidof(p_Device), (void**)&p_Device);
	if (nullptr == p_Device) { return false; }

	p_Device->GetParent(__uuidof(IDXGIAdapter), (void**)&p_Adapter);
	if (nullptr == p_Adapter) { return false; }

	p_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&p_Factory);
	if (nullptr == p_Factory) { return false; }

	// ���⼭ ����ü���� �޾ƿ��� �����̴�.
	if (nullptr != p_Factory && S_OK != p_Factory->CreateSwapChain(m_pDevice, &m_Decs, &m_pSwapChain))
	{
		return false;
	}

	// �� �Ķ�� �ʿ䰡 ���� ������ ����
	p_Factory->Release();
	p_Adapter->Release();
	p_Device->Release();

	return true;
}

// ����ü�ο� ���� �ؽ�ó�� ���⼭ �����.
// ��ȭ���� ����ٴ� �����̴�.
bool KDevice::Create_View()
{
	m_pBackBuffer = nullptr;
	if(S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer))
	{
		return false;
	}

	// Ÿ�ٺ並 �δܰ踦 ���ļ� �����´�.�̷���...
	if (S_OK != m_pDevice->CreateRenderTargetView(m_pBackBuffer, 0, &m_pTargetView))
	{
		return false;
	}

	// �ٸ� �������� ���̱� ������ ���� ���´�.
	// m_pBackBuffer->Release();



	// �ؽ��� ���� ����
	D3D11_TEXTURE2D_DESC m_Decs = {};

	m_Decs.ArraySize = 1;

	// ���⼭ ������ ����� ȭ�鿡 �ݹۿ� �ȳ��´ٰ�..��������
	m_Decs.Width = (UINT)window()->width();
	m_Decs.Height = (UINT)window()->heigth();
	m_Decs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 24 ��Ʈ�� float���� ���� üũ
	// �ڿ� 8 ��Ʈ�� int�� ��Ʈ���� �������� ���ٽ� ���� üũ

	m_Decs.SampleDesc.Count = m_iMSLv;
	m_Decs.SampleDesc.Quality = m_iMSLv - 1;

	// �Ӹ� ����
	m_Decs.MipLevels = 1;

	m_Decs.Usage = D3D11_USAGE_DEFAULT;
	m_Decs.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	

	m_pDevice->CreateTexture2D(&m_Decs, 0, &m_pTexture2D);

	if (nullptr == m_pTexture2D)
	{
		return false;
	}

	if (S_OK != m_pDevice->CreateDepthStencilView(m_pTexture2D, 0, &m_pDS_View))
	{
		return false;
	}
	
	// ������ ������ ���ΰ� �����ִٴ� ... ���� �϶��
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDS_View);

	return true;
}

bool KDevice::Create_ViewPort()
{
	D3D11_VIEWPORT m_Decs = {};

	m_Decs.TopLeftX = 0;
	m_Decs.TopLeftY = 0;

	m_Decs.Width = (float)window()->width();
	m_Decs.Height = (float)window()->heigth();

	m_Decs.MinDepth = 0;
	m_Decs.MaxDepth = 1;

	m_pContext->RSSetViewports(1, &m_Decs);

	return true;
}





/***************** Raster ***************/
KPtr<KDevice::RasterState> KDevice::Find_RasterMode(const wchar_t* _Name)
{
	return Map_Find<KPtr<RasterState>>(m_RasterMap, _Name);
}

void KDevice::Create_RasterMode(const wchar_t* _Name, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull)
{
	RasterState* TempRaster = new RasterState();
	TempRaster->Create(m_pDevice, m_pContext, _Fill, _Cull);
	m_RasterMap.insert(std::unordered_map<std::wstring, KPtr<RasterState>>::value_type(_Name, TempRaster));
}



void KDevice::Set_RasterMode(const wchar_t* _Name)
{
	KPtr<RasterState> RSS = Map_Find<KPtr<RasterState>>(m_RasterMap, _Name);
	if (nullptr == RSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == RSS->m_RS)
	{
		KASSERT(true);
		return;
	}

	m_RState_Def =  RSS;	
	m_RState_Def->Update();
	return;
}

void KDevice::ResetRasterState()
{
	m_RState_Def->Update();
}

void KDevice::RasterState::Update()
{
	m_Context->RSSetState(m_RS);
}

void KDevice::RasterState::Create(ID3D11Device* _Device, ID3D11DeviceContext* _Context
	, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull)
{
	if (nullptr == _Context)
	{
		KASSERT(true);
		return;
	}

	m_Desc.FillMode = _Fill;
	m_Desc.CullMode = _Cull;
	m_Context = _Context;

	if (S_OK != _Device->CreateRasterizerState(&m_Desc, &m_RS))
	{
		return;
	}
}



/******************** Depth Stencil ********************/
// Raster ����
KPtr<KDevice::DepthStencilState> KDevice::Find_DepthStencil(const wchar_t* _Name)
{
	return Map_Find<KPtr<DepthStencilState>>(m_DepthStencilMap, _Name);
}

void KDevice::Create_DepthSencil(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	DepthStencilState* TempDS = new DepthStencilState();
	TempDS->Create(m_pDevice, m_pContext, _Desc);
	m_DepthStencilMap.insert(std::unordered_map<std::wstring, KPtr<DepthStencilState>>::value_type(_Name, TempDS));
}



void KDevice::Set_DepthSencil(const wchar_t* _Name)
{
	KPtr<DepthStencilState> DSS = Map_Find<KPtr<DepthStencilState>>(m_DepthStencilMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == DSS->m_DSS)
	{
		KASSERT(true);
		return;
	}

	m_DState_Def = DSS;
	m_DState_Def->Update();
	return;
}

void KDevice::Set_DepthSencilMode(const wchar_t* _Name)
{
	KPtr<DepthStencilState> DSS = Map_Find<KPtr<DepthStencilState>>(m_DepthStencilMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}
	DSS->Update();
}

void KDevice::Reset_DepthStencil()
{
	m_DState_Def->Update();
}

void KDevice::DepthStencilState::Update()
{
	m_Context->OMSetDepthStencilState(m_DSS, 0);
}

void KDevice::DepthStencilState::Create(ID3D11Device* _Device, ID3D11DeviceContext* _Context, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	if (nullptr == _Context)
	{
		KASSERT(true);
		return;
	}
	m_Context = _Context;

	if (S_OK != _Device->CreateDepthStencilState(&m_Desc, &m_DSS))
	{
		return;
	}
}


// ������ ���ִ� �Լ�
void KDevice::Clear_Target()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_Color.s);
	m_pContext->ClearDepthStencilView(m_pDS_View, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

// ����ִ� �Լ�
void KDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}


void KDevice::Reset_Context()
{
	m_pContext->VSSetShader(nullptr, nullptr, 0);
	m_pContext->HSSetShader(nullptr, nullptr, 0);
	m_pContext->GSSetShader(nullptr, nullptr, 0);
	m_pContext->CSSetShader(nullptr, nullptr, 0);
	m_pContext->PSSetShader(nullptr, nullptr, 0);
}


void KDevice::SetOM()
{
	// ���ڳ� �ٸ� �������� ���ٽ��� �ǵ帱 ��� �ٽ� ���������� ��� ����
	// ��Ȯ���� ���� ������� ���� ���׹��׵� ���̰��� ������
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDS_View);
}


bool KDevice::Create_DeviceCB(Device_CB* _NewBuf)
{
	if (S_OK != Core_Class::Device()->CreateBuffer(&_NewBuf->tDesc, nullptr, &_NewBuf->p_ConstBuffer))
	{
		delete _NewBuf;
		return false;
	}
	return true;
}

KPtr<KDevice::Device_CB> KDevice::Find_DeviceCB(const wchar_t* _Name)
{
	return Map_Find<KPtr<KDevice::Device_CB>>(m_CB_Map, _Name);
}