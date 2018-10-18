#include "HDevice.h"
#include "HWindow.h"
#include "Stl_AID.h"
#include "SmartPtr.h"
#include "HBlend.h"


HDevice::HDevice(HWindow* _Win) : HWindowBase(_Win),
m_pDevice(nullptr),
m_pContext(nullptr),
m_pTagetView(nullptr),
m_pDepthStencilTex(nullptr),
m_pDepthStencilView(nullptr),
m_pSwapChain(nullptr),
m_bInit(false),
m_Color(0.0f, 0.5f, 0.0f, 1.0f)
{
}


HDevice::~HDevice()
{
	Release();
}

void HDevice::Release()
{
	if (nullptr != m_pDepthStencilView) { m_pDepthStencilView->Release(); }
	if (nullptr != m_pDepthStencilTex) { m_pDepthStencilTex->Release(); }
	if (nullptr != m_pBackBuffer) { m_pBackBuffer->Release(); }
	if (nullptr != m_pTagetView) { m_pTagetView->Release(); }
	if (nullptr != m_pSwapChain) { m_pSwapChain->Release(); }
	if (nullptr != m_pContext) { m_pContext->Release(); }
	if (nullptr != m_pDevice) { m_pDevice->Release(); }
}


bool HDevice::Init()
{

	m_bInit = false;

	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 디바이스의 현재 버전을 알아보자.
	// 얻어오는것.
	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE 라고 했다면 3D 외부 3D장치를 찾는다.
	// 소프트

	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
		, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &eLv, &m_pContext))
	{
		Release();
		return false;
	}


	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iMSLv);

	// 멀티 샘플링 레벨을 체크한다.
	// 가져온것이고.

	// 0이라는 것은 멀티샘플링 자체를 지원 안한다는 것이기 때문에
	// 그때는 터트립니다.
	//if (0 == m_iMSLv)
	//{
	//	return false;
	//}

	if (false == CreateSwapChain())
	{
		Release();
		return false;
	}


	if (false == CreateView())
	{
		Release();
		return false;
	}

	if (false == CreateViewPort())
	{
		Release();
		return false;
	}


	m_bInit = true;
	return true;
}

bool HDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC tDecs = {};

	// 백버퍼의 크기
	tDecs.BufferDesc.Width = (UINT)Window()->Width();
	tDecs.BufferDesc.Height = (UINT)Window()->Height();

	// 버퍼 갱신 주기및 버퍼의 개수
	tDecs.BufferDesc.RefreshRate.Numerator = 60;
	tDecs.BufferDesc.RefreshRate.Denominator = 1;

	// 버퍼 픽샐
	tDecs.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 스캐닝 라인 세팅
	// 스캔라인
	tDecs.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDecs.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 버퍼의 용도가 뭐에요.
	tDecs.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;

	tDecs.OutputWindow = Window()->WINHWND();

	tDecs.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	tDecs.BufferCount = 1;

	// 전체화면을 쓰느냐 마느냐?
	tDecs.Windowed = !Window()->IsFull();

	// 이녀석들은 추후 설명하겠습니다.
	// 스왑체인을 만들기 위해서 얻어다 줘야 하는 녀석들이라고
	IDXGIDevice* pIdxDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	m_pDevice->QueryInterface(__uuidof(pIdxDevice), (void**)&pIdxDevice);
	if (nullptr == pIdxDevice)
	{
		return false;
	}

	pIdxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	if (nullptr == pAdapter)
	{
		return false;
	}
	// 만들수 스왑체인을 만들수 있는 단계의 인터페이스까지 가져온다.
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	if (nullptr != pFactory && S_OK != pFactory->CreateSwapChain(m_pDevice, &tDecs, &m_pSwapChain))
	{
		return false;
	}

	pFactory->Release();
	pAdapter->Release();
	pIdxDevice->Release();

	return true;

}

// 스완체인에 사용될 텍스처를 여기서 만들어 준다.
bool HDevice::CreateView()
{
	m_pBackBuffer = nullptr;
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer))
	{
		return false;
	}

	// 얻어온 택스처로 실제 랜더를 하기 위한 View를 만든다.

	if (S_OK != m_pDevice->CreateRenderTargetView(m_pBackBuffer, 0, &m_pTagetView))
	{
		return false;
	}

	// pBackBufferTex->Release();

	// 텍스처 정보 세팅을 해줄수 있는 녀석
	D3D11_TEXTURE2D_DESC tDecs = {};

	tDecs.ArraySize = 1;
	tDecs.Width = (UINT)Window()->Width();
	tDecs.Height = (UINT)Window()->Height();
	tDecs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 앞의 24비트는 float을 통해서 깊이를 체크하고
	// 뒤의 8비트는 int형으로 비트단위 연산등을 통한
	// 스탠실버퍼를 체크하는 용도이다.

	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;

	tDecs.MipLevels = 1;

	// 기본 텍스처 용도로 그대로 세팅해달라고 말하는것./
	// 스탠실 퍼버라는 것으로 쓰겠다.
	tDecs.Usage = D3D11_USAGE_DEFAULT;
	tDecs.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDevice->CreateTexture2D(&tDecs, 0, &m_pDepthStencilTex);

	if (nullptr == m_pDepthStencilTex)
	{
		return true;
	}


	if (S_OK != m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, 0, &m_pDepthStencilView))
	{
		return true;
	}

	// 이게 랜더링 파이프 라인과 관련이 있습니다.
	// RenderTargetView 와 DepthStencilView 를 같이 묶어서 OM 단계에 전달한다.	
	// 고정기능단계

	// 이중포인터면 대부분 다중 세팅을 하려고 하는것.
	m_pContext->OMSetRenderTargets(1, &m_pTagetView, m_pDepthStencilView);

	return true;
}

bool HDevice::CreateViewPort()
{
	D3D11_VIEWPORT tDecs = {};
	tDecs.TopLeftX = 0;
	tDecs.TopLeftY = 0;

	tDecs.Width = (float)Window()->Width();
	tDecs.Height = (float)Window()->Height();

	tDecs.MinDepth = 0;
	tDecs.MaxDepth = 1;

	m_pContext->RSSetViewports(1, &tDecs);

	return true;
}



// 새로 그리기 전에 깨끗이 만들어라.
void HDevice::ClearTaget()
{
	m_pContext->ClearRenderTargetView(m_pTagetView, m_Color.s);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
} 

// 출력하라
void HDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}

void HDevice::ResetContext() {
	m_pContext->VSSetShader(nullptr, nullptr, 0);
	m_pContext->HSSetShader(nullptr, nullptr, 0);
	m_pContext->GSSetShader(nullptr, nullptr, 0);
	m_pContext->CSSetShader(nullptr, nullptr, 0);
	m_pContext->PSSetShader(nullptr, nullptr, 0);
}

void HDevice::OMSet() 
{
	m_pContext->OMSetRenderTargets(1, &m_pTagetView, m_pDepthStencilView);
	// m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}

///////////////////////////////////////////// CB

bool HDevice::CreateCB(GCBUFFER* NewBuf) 
{
	if (S_OK != m_pDevice->CreateBuffer(&NewBuf->tDesc, nullptr, &NewBuf->pConstBuffer))
	{
		delete NewBuf;
		return false;
	}

	return true;
}

KPtr<HDevice::GCBUFFER> HDevice::FindCB(const wchar_t* _Name)
{
	return Map_Find<KPtr<HDevice::GCBUFFER>>(m_MapConstBuffer, _Name);
}

////////////////////////////////////////// RS

KPtr<HDevice::RSState> HDevice::FindRsMode(const wchar_t* _Name)
{
	return Map_Find<KPtr<RSState>>(m_RSMap, _Name);
}

void HDevice::CreateRsMode(const wchar_t* _Name, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode)
{
	RSState* _Ptr = new RSState();
	_Ptr->Create(m_pDevice, m_pContext, _FillMode, _CullMode);
	m_RSMap.insert(std::unordered_map<std::wstring, KPtr<RSState>>::value_type(_Name, _Ptr));
}

void HDevice::SetDefRsMode(const wchar_t* _Name)
{
	KPtr<RSState> RSS = Map_Find<KPtr<RSState>>(m_RSMap, _Name);
	if (nullptr == RSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == RSS->m_pRS)
	{
		KASSERT(true);
		return;
	}
	m_DefaultRState = RSS;
	m_DefaultRState->Update();
	return;
}

void HDevice::ResetRSState()
{
	m_DefaultRState->Update();
}

void HDevice::RSState::Update()
{
	m_pContext->RSSetState(m_pRS);
}


void HDevice::RSState::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_FILL_MODE _FillMode, D3D11_CULL_MODE _CullMode)
{
	if (nullptr == _pContext)
	{
		KASSERT(nullptr == _pContext);
		return;
	}

	// 꼭말해달라고 해주세요.
	m_Desc.FillMode = _FillMode;
	m_Desc.CullMode = _CullMode;
	m_pContext = _pContext;

	if (S_OK != _pDevice->CreateRasterizerState(&m_Desc, &m_pRS))
	{
		return;
	}
}

void HDevice::SetRsMode(const wchar_t* _Name) 
{
	KPtr<RSState> RSS = Map_Find<KPtr<RSState>>(m_RSMap, _Name);
	if (nullptr == RSS)
	{
		KASSERT(true);
		return;
	}
	RSS->Update();
}

////////////////////////////////////////// DS
KPtr<HDevice::DSState> HDevice::FindDsMode(const wchar_t* _Name)
{
	return Map_Find<KPtr<DSState>>(m_DSMap, _Name);
}

void HDevice::CreateDsMode(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	DSState* _Ptr = new DSState();
	_Ptr->Create(m_pDevice, m_pContext, _Desc);
	m_DSMap.insert(std::unordered_map<std::wstring, KPtr<DSState>>::value_type(_Name, _Ptr));
}

void HDevice::SetDefDsMode(const wchar_t* _Name)
{
	KPtr<DSState> DSS = Map_Find<KPtr<DSState>>(m_DSMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == DSS->m_pDS)
	{
		KASSERT(true);
		return;
	}
	m_DefaultDState = DSS;
	m_DefaultDState->Update();
	return;
}

void HDevice::ResetDSState()
{
	m_DefaultDState->Update();
}

void HDevice::DSState::Update(unsigned int _Ref)
{
	// 이건 뎁스 스탠실 값
	m_pContext->OMSetDepthStencilState(m_pDS, _Ref);
}

void HDevice::DSState::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	if (nullptr == _pContext)
	{
		KASSERT(nullptr == _pContext);
		return;
	}

	m_pContext = _pContext;

	if (S_OK != _pDevice->CreateDepthStencilState(&_Desc, &m_pDS))
	{
		return;
	}
}

void HDevice::SetDsMode(const wchar_t* _Name, unsigned int _Ref)
{
	KPtr<DSState> DSS = Map_Find<KPtr<DSState>>(m_DSMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}
	DSS->Update(_Ref);
}

void HDevice::SetBsMode(const wchar_t* _Name) 
{
	KPtr<HBlend> Ptr = HResMgr<HBlend>::Find(_Name);

	if (nullptr == Ptr)
	{
		BBY;
	}
	Ptr->Update();
}