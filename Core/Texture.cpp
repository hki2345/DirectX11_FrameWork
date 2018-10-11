#include "Texture.h"
#include "Core_Class.h"
#include "KMacro.h"


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
	m_Texture2D(nullptr), 
	m_pSRV(nullptr),
	m_pRTV(nullptr),
	m_pDSV(nullptr)
{
}


Texture::~Texture()
{
	if (nullptr != m_Texture2D) { m_Texture2D->Release(); }
	if (nullptr != m_pSRV) { m_pSRV->Release(); }
	if (nullptr != m_pRTV) { m_pRTV->Release(); }
	if (nullptr != m_pDSV) { m_pDSV->Release(); }
}

//  Ȯ���� ���� �ٸ� �Լ��� ���� -> ���̷�Ʈ���� �׷��� �ϴϱ�
// �� �翬�� �Űڴ�.
// �� ���� Ȯ���ڴ� ���Ͽ� ���� �� Ȯ���ڸ� �����ϱ� ���� ������ �ڵ常 �ٸ���
// ���̳ʸ� ���´� ����. (��ȣȭ���� ����ٸ�) �ϸ� �� ��ȣȭ �ؾ��ϴ� �Ű�
bool Texture::Load()
{
	// Ȯ���ڸ� �˻� - DDS -  ���̷�Ʈ ���� �̹��� ����
	// ���� ���̴� ���� ���ʿ� ��ġ ����
	if (true == is_Exist(L".dds") || true == is_Exist(L".DDS") || true == is_Exist(L".DDs"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(path(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}
	else if (true == is_Exist(L".tga") || true == is_Exist(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(path(), nullptr, m_Image))
		{
			return false;
		}
	}
	else //png, bmp, jpgȮ���ڴ� else�� ���� ���� 
	{
		if (S_OK != DirectX::LoadFromWICFile(path(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}

	// �Τм���
	if (S_OK != DirectX::CreateShaderResourceView(Core_Class::device(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		&m_pSRV))
	{
		return false;
	}
	
	KASSERT(nullptr == m_pSRV);

	m_pSRV->GetResource((ID3D11Resource**)(&m_Texture2D));

	KASSERT(nullptr == m_Texture2D);
	return true;
}

void Texture::Update(const UINT& _Slot)
{
	Core_Class::device_context()->VSSetShaderResources(_Slot, 1, &m_pSRV);
	Core_Class::device_context()->PSSetShaderResources(_Slot, 1, &m_pSRV);
}

void Texture::Reset(const KUINT& _Slot)
{
	ID3D11ShaderResourceView* RV = nullptr;
	Core_Class::device_context()->VSSetShaderResources(_Slot, 1, &RV);
	Core_Class::device_context()->PSSetShaderResources(_Slot, 1, &RV);
}

bool Texture::Create(const UINT& _W, const UINT& _H, const UINT& _BindFlag, DXGI_FORMAT _eForm,
	D3D11_USAGE _eUsage /*= D3D11_USAGE::D3D11_USAGE_DEFAULT*/)
{
	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.Width = _W;
	tDesc.Height = _H;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = _BindFlag;
	tDesc.Usage = _eUsage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC== _eUsage)
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		tDesc.CPUAccessFlags = 0;
	}

	tDesc.Format = _eForm;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.MipLevels = 1;

	if (S_OK != Core_Class::device()->CreateTexture2D(&tDesc, nullptr, &m_Texture2D))
	{
		KASSERT(true);
		return false;
	}

	Set_View(_BindFlag);

	return true;
}

bool Texture::Create(ID3D11Texture2D* _Tex, UINT _BindFlag)
{
	return true;
}

void Texture::Set_View(UINT _BindFlag)
{
	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & _BindFlag)
	{
		if (S_OK != Core_Class::device()->CreateDepthStencilView(m_Texture2D, 0, &m_pDSV))
		{
			KASSERT(true);
			return;
		}
	}
	else
	{
		if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & _BindFlag)
		{
			if (S_OK != Core_Class::device()->CreateRenderTargetView(m_Texture2D, 0, &m_pRTV))
			{
				KASSERT(true);
				return;
			}
		}
		if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & _BindFlag)
		{
			if (S_OK != Core_Class::device()->CreateShaderResourceView(m_Texture2D, 0, &m_pSRV))
			{
				KASSERT(true);
				return;
			}
		}
	}

	if (S_OK != DirectX::CaptureTexture(Core_Class::device(), Core_Class::device_context(), m_Texture2D, m_Image))
	{
		KASSERT(true);
		return;
	}
}