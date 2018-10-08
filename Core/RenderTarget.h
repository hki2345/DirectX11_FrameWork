#pragma once
#include "Texture.h"


class RenderTarget : public KResource
{
public:
	RenderTarget();
	~RenderTarget();

private:
	KPtr<Texture> m_Texture;

public:
	KPtr<Texture> texture()
	{
		return m_Texture;
	}

	// UINT 보단 unsigned int를 손에 익히자 -> UINT는 windows.h를 필요로하는데
	// 개발 환경에 따라 없을 수도 있으니까.
public:
	bool Create(const unsigned int& _W, const unsigned int& _H, const unsigned int& _BindFlag,
		DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, const unsigned int& _BindFlag);
};

