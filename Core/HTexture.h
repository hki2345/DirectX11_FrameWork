#pragma once
#include "HRes.h"
#include "DHeader.h"
#include "DirectXTex.h"


class HTexture : public HRes
{
private:
	DirectX::ScratchImage			m_Image;
	ID3D11Texture2D*				m_pTex2D;

	// 쉐이더에 텍스처를 넣어줄때 이녀석으로 넣어준다.
	ID3D11ShaderResourceView*		m_pSRV;
	ID3D11RenderTargetView*			m_pRTV;
	ID3D11DepthStencilView*			m_pDSV;

public:
	float Width() { return (float)m_Image.GetMetadata().width; }
	float Height() { return (float)m_Image.GetMetadata().height; }
	HVEC2 ImageSize() { return { Width(), Height() }; }
	HVEC GetPixel(int _X, int _Y);


	// ViewSetting
	ID3D11ShaderResourceView*		SRV() { return m_pSRV; }
	ID3D11RenderTargetView*			RTV() { return m_pRTV; }
	ID3D11DepthStencilView*			DSV() { return m_pDSV; }

public:
	void ViewSetting(UINT _BindFlag);

	bool Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag);

public:
	bool Load();

public:
	void Update(unsigned int _Slot);
	void Reset(unsigned int _Slot);

public:
	HTexture();
	~HTexture();
};

