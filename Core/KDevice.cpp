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
m_pDepthStencilView(nullptr),
m_pSwapChain(nullptr),
m_bInit(false),
m_Color(.5f, .5f, .5f, 1.0f),
m_AllDefaultStateSetting(false),

m_pDepthStencilState(nullptr),
m_pDepthStencilStateDeg(nullptr)
{
}

KDevice::~KDevice()
{
	Release();
}
bool KDevice::Init()
{
	m_bInit = false;
	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 어떤 버전으로 디바이스를 구성할 것이냐-- 근데 자동으로해주니까 별 그게 없다.
	// 용도가 현재 버전이 어떤것인지 찾는 것이다.
	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE 라고 했다면 3D 외부 3D장치를 찾는다. 
	// -> 글카를 찾는다는 뜻
	// 소프트일 경우는 CPU로 연산 처리 - 생각만 해도 겁나느릴 것 같지 않냐 ㅋㅋㅋㅋ

	//  
	if (S_OK != D3D11CreateDevice(
		/*특수상황*/nullptr, /*외부장치*/D3D_DRIVER_TYPE_HARDWARE
		, /*외부모듈*/nullptr, /*Flag*/ iFlag, 
		/*??*/nullptr,/*??*/ 0,/*SDK 버젼*/ D3D11_SDK_VERSION, 
		/*받아올 장치*/&m_pDevice, /*받아올 현재 버젼*/&eLv, 
		/*받아올 Context - 설명은 나중에한댔는데 언젠인지는 기약 없음*/&m_pContext))
	{
		Release();
		return false;
	}

	// 멀티 샘플링 레벨을 체크한다.
	// 몇단계로 부드럽게 할 것이냐 -> 몇 단계로 확대시킨 이미지를 합성할 것이냐
	// 여기서는 4단계로 하였다.
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iMSLv);

	// 반환값이 0이면 에러 발생한 것임 - 당연 안되야징
	if (0 == m_iMSLv) { Release(); return false; }

	if (false == Create_SwapChain()) { Release(); return false; }
	if (false == Create_View()) { Release(); return false; }
	if (false == Create_ViewPort()) { Release(); return false; }
	

	m_bInit = true;
	return true;
}


bool KDevice::Create_SwapChain()
{
	// 뭔 을 가진 구조체다. -> 이것들을 모두 초기화해주어야 한닿ㅎㅎㅎㅎ
	// 역시 그냥 되는 게 아니고... 어디를 타고 타고 타고 가서 초기화 하는 식
	/*DXGI_MODE_DESC BufferDesc;
	DXGI_SAMPLE_DESC SampleDesc;
	DXGI_USAGE BufferUsage;
	UINT BufferCount;
	HWND OutputWindow;
	BOOL Windowed;
	DXGI_SWAP_EFFECT SwapEffect;
	UINT Flags;*/
	DXGI_SWAP_CHAIN_DESC m_Decs = {};

	// 백버퍼 크기
	m_Decs.BufferDesc.Width = (UINT)window()->width();
	m_Decs.BufferDesc.Height = (UINT)window()->heigth();

	// 백버퍼 주사율 + 백버퍼 개수
	m_Decs.BufferDesc.RefreshRate.Numerator = 60;
	m_Decs.BufferDesc.RefreshRate.Denominator = 1;

	// 버퍼의 픽샐 - 어떠한 색 집합으로 버퍼의 픽샐을 채우겠다.
	m_Decs.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 스캔 라인 스케일 세팅 - 주말 공부
	m_Decs.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_Decs.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 버퍼의 용도 - 화면 사출
	m_Decs.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;


	// 셈플링 적용 - 이건 이전 단계에서 몇 단계로 지원하는지 받아온 뒤이다.
	m_Decs.SampleDesc.Count = m_iMSLv;
	m_Decs.SampleDesc.Quality = m_iMSLv - 1;


	// 윈도우 세팅
	m_Decs.OutputWindow = window()->KWnd();

	// 스왑할때 이펙트- 우린 지운다.
	m_Decs.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 버퍼 개수
	m_Decs.BufferCount = 1;

	// 전체화면 여부 - 해당 윈도우의 우선순위 __ 전체화면이 최우선
	m_Decs.Windowed = !window()->Is_Full();


	/*********** 추후 설명 *************/ // 컴객체와 관련있음ㅇㅇ
	IDXGIDevice* p_Device = nullptr;
	IDXGIAdapter* p_Adapter = nullptr;
	IDXGIFactory* p_Factory = nullptr;

	m_pDevice->QueryInterface(__uuidof(p_Device), (void**)&p_Device);
	if (nullptr == p_Device) { return false; }

	p_Device->GetParent(__uuidof(IDXGIAdapter), (void**)&p_Adapter);
	if (nullptr == p_Adapter) { return false; }

	p_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&p_Factory);
	if (nullptr == p_Factory) { return false; }

	// 여기서 스왑체인을 받아오기 위함이다.
	if (nullptr != p_Factory && S_OK != p_Factory->CreateSwapChain(m_pDevice, &m_Decs, &m_pSwapChain))
	{
		return false;
	}

	// 한 후라면 필요가 없기 때문에 해제
	p_Factory->Release();
	p_Adapter->Release();
	p_Device->Release();

	return true;
}

// 스왑체인에 사용될 텍스처를 여기서 찍언댄다.
// 도화지를 만든다는 느낌이다.
bool KDevice::Create_View()
{
	m_pBackBuffer = nullptr;
	if(S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer))
	{
		return false;
	}

	// 타겟뷰를 두단계를 걸쳐서 가져온다.이렇게...
	if (S_OK != m_pDevice->CreateRenderTargetView(m_pBackBuffer, 0, &m_pTargetView))
	{
		return false;
	}

	// 다른 곳에서도 쓰이기 때문에 남겨 놓는다.
	// m_pBackBuffer->Release();



	// 텍스쳐 정보 세팅
	D3D11_TEXTURE2D_DESC m_Decs = {};

	m_Decs.ArraySize = 1;

	// 여기서 반으로 만들면 화면에 반밖에 안나온다고..ㅋㅋㅋㅋ
	m_Decs.Width = (UINT)window()->width();
	m_Decs.Height = (UINT)window()->heigth();
	m_Decs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	m_Decs.SampleDesc.Count = m_iMSLv;
	m_Decs.SampleDesc.Quality = m_iMSLv - 1;

	// 밉맵 설정
	m_Decs.MipLevels = 1;

	m_Decs.Usage = D3D11_USAGE_DEFAULT;
	m_Decs.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	

	m_pDevice->CreateTexture2D(&m_Decs, 0, &m_pTexture2D);

	if (nullptr == m_pTexture2D)
	{
		return false;
	}

	if (S_OK != m_pDevice->CreateDepthStencilView(m_pTexture2D, 0, &m_pDepthStencilView))
	{
		return false;
	}


	m_DepthDESC.DepthEnable = TRUE;
	m_DepthDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	// 뎁스 비교연산을 하지 않겠다.
	m_DepthDESC.DepthFunc = D3D11_COMPARISON_LESS;
	m_DepthDESC.StencilEnable = FALSE;
	m_DepthDESC.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_DepthDESC.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//////////////////////////////Debug
	const D3D11_DEPTH_STENCILOP_DESC defDebStaencilOp =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };

	m_DepthDESC.FrontFace = defDebStaencilOp;
	m_DepthDESC.BackFace = defDebStaencilOp;

	m_pDevice->CreateDepthStencilState(&m_DepthDESC, &m_pDepthStencilStateDeg);

	if (nullptr == m_pDepthStencilStateDeg)
	{
		KASSERT(true);
		return false;
	}
	//////////////////////////////////


	m_DepthDESC.DepthEnable = TRUE;
	m_DepthDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_DepthDESC.DepthFunc = D3D11_COMPARISON_ALWAYS;
	m_DepthDESC.StencilEnable = FALSE;
	m_DepthDESC.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_DepthDESC.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	const D3D11_DEPTH_STENCILOP_DESC defStaencilOp =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };

	m_DepthDESC.FrontFace = defStaencilOp;
	m_DepthDESC.BackFace = defStaencilOp;

	m_pDevice->CreateDepthStencilState(&m_DepthDESC, &m_pDepthStencilState);

	if (nullptr == m_pDepthStencilState)
	{
		KASSERT(true);
		return false;
	}

	
	// 랜더링 파이프 라인과 관련있다는 ... 공부 하라능
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthStencilView);
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

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



void KDevice::Create_RasterMode(const wchar_t* _Name)
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

	m_DefaultState =  RSS;	
	m_DefaultState->Update();
	return;
}

void KDevice::ResetRasterState()
{
	m_DefaultState->Update();
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

// 깨끗이 해주는 함수
void KDevice::Clear_Target()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_Color.s);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

// 찍어주는 함수
void KDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}

void KDevice::Release()
{
	if (nullptr != m_pDepthStencilStateDeg) { m_pDepthStencilStateDeg->Release(); }
	if (nullptr != m_pDepthStencilState) { m_pDepthStencilState->Release(); }
	if (nullptr != m_pDepthStencilView) { m_pDepthStencilView->Release(); }
	if (nullptr != m_pTargetView) { m_pTargetView->Release(); }
	if (nullptr != m_pSwapChain) { m_pSwapChain->Release(); }
	if (nullptr != m_pBackBuffer) { m_pBackBuffer->Release(); }
	if (nullptr != m_pTexture2D) { m_pTexture2D->Release(); }
	if (nullptr != m_pDevice) { m_pDevice->Release(); }
	if (nullptr != m_pContext) { m_pContext->Release(); }
}

void KDevice::reset_context()
{
	m_pContext->VSSetShader(nullptr, nullptr, 0);
	m_pContext->HSSetShader(nullptr, nullptr, 0);
	m_pContext->GSSetShader(nullptr, nullptr, 0);
	m_pContext->CSSetShader(nullptr, nullptr, 0);
	m_pContext->PSSetShader(nullptr, nullptr, 0);
}

void KDevice::reset_depthstencil()
{
	// 글자나 다른 렌더러가 스텐실을 건드릴 경우 다시 렌더순서를 잡기 위햄
	// 정확히는 랜더 순서라기 보다 뒤죽박죽된 깊이값을 재조정
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthStencilView);
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}


bool KDevice::Create_DeviceCB(Device_CB* _NewBuf)
{
	if (S_OK != Core_Class::device()->CreateBuffer(&_NewBuf->tDesc, nullptr, &_NewBuf->p_ConstBuffer))
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