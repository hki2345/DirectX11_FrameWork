#pragma once
#include "Resource.h"
#include "DXContainer.h"
#include "DirectXTex.h"


class Texture : public Resource
{
public:
	friend class Texture_Multi;

public:
	Texture();
	~Texture();

private:
	DirectX::ScratchImage			m_Image;
	ID3D11Texture2D*				m_pTex2D;

	// 쉐이더에 텍스처를 넣어줄때 이녀석으로 넣어준다.
	// 텍스쳐에 넣어줄 세이더
	ID3D11ShaderResourceView*		m_pSRV;
	ID3D11RenderTargetView*			m_pRTV;
	ID3D11DepthStencilView*			m_pDSV;

	// 텍스쳐를 간헐적으로 데이터를 찍기위함 
	D3D11_MAPPED_SUBRESOURCE SubData;

public:
	float Width() { return (float)m_Image.GetMetadata().width; }
	float Height() { return (float)m_Image.GetMetadata().height; }
	KVector2 ImageSize() { return { Width(), Height() }; }
	KVector4 GetPixel(int _X, int _Y);


	// Set_View
	ID3D11ShaderResourceView*		SRV() { return m_pSRV; }
	ID3D11RenderTargetView*			RTV() { return m_pRTV; }
	ID3D11DepthStencilView*			DSV() { return m_pDSV; }

public:
	void Set_View(UINT _BindFlag);

	bool Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag);

public:
	void Set_Pixel(void* Src, size_t _Size);
	bool Load();

public:
	void Update(const KUINT& _Slot);
	void Reset(const KUINT& _Slot);

};

