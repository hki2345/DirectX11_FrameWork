#include "HTexture.h"
#include "HVAR.h"
#include "KMacro.h"
#include "MCRI.h"



#if X64
#if _DEBUG
#pragma comment(lib, "DirectXTex_X64_DEBUG")
#else if NDEBUG
#pragma comment(lib, "DirectXTex_X64_RELEASE")
#endif
#else if WIN32
#if _DEBUG
#pragma comment(lib, "DirectXTex_X86_DEBUG")
#else if NDEBUG
#pragma comment(lib, "DirectXTex_X86_RELEASE")
#endif
#endif // DEBUG



HTexture::HTexture() : m_pTex2D(nullptr), m_pSRV(nullptr), m_pRTV(nullptr), m_pDSV(nullptr)
{
}


HTexture::~HTexture()
{
	if (nullptr != m_pTex2D)	{		m_pTex2D->Release();	}
	if (nullptr != m_pSRV)		{ m_pSRV->Release(); }
	if (nullptr != m_pRTV)		{ m_pRTV->Release(); }
	if (nullptr != m_pDSV)		{ m_pDSV->Release(); }
}

// 파일헤더
// 파일들은 항상 다 특정한 데이터구조를 헤더에 적어놨다. 
// 파일종류마다 로딩함수가 다르다.
bool HTexture::Load() 
{
	MCRI A{ 0 };

	// bmp
	if (true == IsExt(L".DDs") || true == IsExt(L".dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(Path(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}
	else if(true == IsExt(L".tga") || true == IsExt(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(Path(), nullptr, m_Image))
		{
			return false;
		}
	}
	else {
		// pNG BMP JPG
		if (S_OK != DirectX::LoadFromWICFile(Path(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}

	if (S_OK != DirectX::CreateShaderResourceView(HVAR::PDevice(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		&m_pSRV))
	{
		return false;
	}

	KASSERT(nullptr == m_pSRV);

	m_pSRV->GetResource((ID3D11Resource**)(&m_pTex2D));

	KASSERT(nullptr == m_pTex2D);
	return true;
}

void HTexture::Update(unsigned int _Slot)
{
	HVAR::Context()->VSSetShaderResources(_Slot, 1, &m_pSRV);
	HVAR::Context()->PSSetShaderResources(_Slot, 1, &m_pSRV);
}

void HTexture::Reset(unsigned int _Slot)
{
	ID3D11ShaderResourceView* Reset = nullptr;
	HVAR::Context()->VSSetShaderResources(_Slot, 1, &Reset);
	HVAR::Context()->PSSetShaderResources(_Slot, 1, &Reset);
}

HVEC HTexture::GetPixel(int _X, int _Y) 
{
	size_t Size = DHelper::GetFmSize(m_Image.GetMetadata().format);

	BaseColor Color;
	uint8_t* p = m_Image.GetPixels();

	p += ((m_Image.GetMetadata().width * _Y) * Size) + (_X * Size);

	memcpy_s(&Color, Size, p, Size);

	HVEC ReturnColor;

	ReturnColor.SetColor(Color.b, Color.g, Color.r, Color.a);

	return ReturnColor;
}

bool HTexture::Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/) 
{
	D3D11_TEXTURE2D_DESC tDecs = {};

	tDecs.Width = _W;
	tDecs.Height = _H;
	tDecs.ArraySize = 1;
	tDecs.BindFlags = _BindFlag;
	tDecs.Usage = _eUsage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _eUsage)
	{
		tDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		tDecs.CPUAccessFlags = 0;
	}

	// MipMap설정
	tDecs.Format = _eFormat;
	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;
	tDecs.MipLevels = 1;

	if (S_OK != HVAR::PDevice()->CreateTexture2D(&tDecs, nullptr, &m_pTex2D))
	{
		BBY;
		return false;
	}

	ViewSetting(_BindFlag);

	return true;
}

bool HTexture::Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag) 
{
	return true;
}

void HTexture::ViewSetting(UINT _BindFlag)
{
	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & _BindFlag)
	{
		if (S_OK != HVAR::PDevice()->CreateDepthStencilView(m_pTex2D, 0, &m_pDSV))
		{
			BBY;
			return;
		}
	}
	else 
	{
		if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & _BindFlag)
		{
			if (S_OK != HVAR::PDevice()->CreateRenderTargetView(m_pTex2D, 0, &m_pRTV))
			{
				BBY;
				return;
			}
		}
		if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & _BindFlag)
		{
			if (S_OK != HVAR::PDevice()->CreateShaderResourceView(m_pTex2D, 0, &m_pSRV))
			{
				BBY;
				return;
			}
		}
	}

	if (S_OK != DirectX::CaptureTexture(HVAR::PDevice(), HVAR::Context(), m_pTex2D, m_Image))
	{
		BBY;
		return;
	}
}

