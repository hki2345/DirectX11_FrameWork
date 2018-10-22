#include "Texture.h"
#include "Core_Class.h"
#include "KMacro.h"
#include "MultiCritical.h"



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



Texture::Texture() :
	m_pTex2D(nullptr),
	m_pSRV(nullptr), 
	m_pRTV(nullptr),
	m_pDSV(nullptr)
{
}


Texture::~Texture()
{
	KRELESE(m_pTex2D);
	KRELESE(m_pSRV);
	KRELESE(m_pRTV);
	KRELESE(m_pDSV);
}

// �������
// ���ϵ��� �׻� �� Ư���� �����ͱ����� ����� �������. 
// ������������ �ε��Լ��� �ٸ���.

//  Ȯ���� ���� �ٸ� �Լ��� ���� -> ���̷�Ʈ���� �׷��� �ϴϱ�
// �� �翬�� �Űڴ�.
// �� ���� Ȯ���ڴ� ���Ͽ� ���� �� Ȯ���ڸ� �����ϱ� ���� ������ �ڵ常 �ٸ���
// ���̳ʸ� ���´� ����. (��ȣȭ���� ����ٸ�) �ϸ� �� ��ȣȭ �ؾ��ϴ� �Ű�
bool Texture::Load() 
{
	MultiCritical A{ 0 };

	// bmp
	if (true == Compare_Ext(L".DDs") || true == Compare_Ext(L".dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(AllPath(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}
	else if(true == Compare_Ext(L".tga") || true == Compare_Ext(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(AllPath(), nullptr, m_Image))
		{
			return false;
		}
	}
	else {
		//png, bmp, jpgȮ���ڴ� else�� ���� ���� 
		if (S_OK != DirectX::LoadFromWICFile(AllPath(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}

	if (S_OK != DirectX::CreateShaderResourceView(Core_Class::PDevice(),
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

void Texture::Update(const KUINT& _Slot)
{
	Core_Class::Context()->VSSetShaderResources(_Slot, 1, &m_pSRV);
	Core_Class::Context()->PSSetShaderResources(_Slot, 1, &m_pSRV);
}

void Texture::Reset(const KUINT& _Slot)
{
	ID3D11ShaderResourceView* Reset = nullptr;
	Core_Class::Context()->VSSetShaderResources(_Slot, 1, &Reset);
	Core_Class::Context()->PSSetShaderResources(_Slot, 1, &Reset);
}

KVector4 Texture::GetPixel(int _X, int _Y) 
{
	size_t Size = VS_Format::Size_Format(m_Image.GetMetadata().format);

	BaseColor Color;
	uint8_t* p = m_Image.GetPixels();

	p += ((m_Image.GetMetadata().width * _Y) * Size) + (_X * Size);

	memcpy_s(&Color, Size, p, Size);

	KVector4 ReturnColor;

	ReturnColor.Set_Color(Color.b, Color.g, Color.r, Color.a);

	return ReturnColor;
}

bool Texture::Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat,
	D3D11_USAGE _eUsage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/) 
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

	// MipMap����
	tDecs.Format = _eFormat;
	tDecs.SampleDesc.Count = 1;
	tDecs.SampleDesc.Quality = 0;
	tDecs.MipLevels = 1;

	if (S_OK != Core_Class::PDevice()->CreateTexture2D(&tDecs, nullptr, &m_pTex2D))
	{
		BBY;
		return false;
	}

	Set_View(_BindFlag);

	return true;
}

bool Texture::Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag) 
{
	return true;
}

void Texture::Set_View(UINT _BindFlag)
{
	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & _BindFlag)
	{
		if (S_OK != Core_Class::PDevice()->CreateDepthStencilView(m_pTex2D, 0, &m_pDSV))
		{
			BBY;
			return;
		}
	}
	else 
	{
		if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & _BindFlag)
		{
			if (S_OK != Core_Class::PDevice()->CreateRenderTargetView(m_pTex2D, 0, &m_pRTV))
			{
				BBY;
				return;
			}
		}
		if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & _BindFlag)
		{
			if (S_OK != Core_Class::PDevice()->CreateShaderResourceView(m_pTex2D, 0, &m_pSRV))
			{
				BBY;
				return;
			}
		}
	}

	if (S_OK != DirectX::CaptureTexture(Core_Class::PDevice(), Core_Class::Context(), m_pTex2D, m_Image))
	{
		BBY;
		return;
	}
}
