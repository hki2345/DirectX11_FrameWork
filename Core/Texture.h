#pragma once
#include "KResource.h"
#include "DirectXTex.h"
#include "DirectXHeader.h"





class Texture : public KResource
{
private:
	DirectX::ScratchImage			m_Image;
	ID3D11Texture2D*				m_Texture2D;

	// 텍스쳐에 넣어줄 세이더
	ID3D11ShaderResourceView*		m_pSRV;
	ID3D11RenderTargetView*			m_pRTV;
	ID3D11DepthStencilView*			m_pDSV;

public:
	bool Load();

	float width() const { return (float)m_Image.GetMetadata().width; }
	float height() const { return (float)m_Image.GetMetadata().height; }
	KVector2 size() const{ return KVector2(width(), height()); }

	// View Setting
	ID3D11ShaderResourceView*	Shader_RescourceView() { return m_pSRV; }
	ID3D11RenderTargetView*		Render_TargetView() { return m_pRTV; }
	ID3D11DepthStencilView*		DepthStencilView() { return m_pDSV; }

	void Set_View(UINT _BindFlag);
	bool Create(const UINT& _W, const UINT& _H, const UINT& _BindFlag, DXGI_FORMAT _eForm,
		D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _Tex, UINT _BindFlag);

public:
	void Update(const KUINT& _Slot);
	void Reset(const KUINT& _Slot);

public:
	Texture();
	~Texture();
};

