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

	// UINT ���� unsigned int�� �տ� ������ -> UINT�� windows.h�� �ʿ���ϴµ�
	// ���� ȯ�濡 ���� ���� ���� �����ϱ�.
public:
	bool Create(const unsigned int& _W, const unsigned int& _H, const unsigned int& _BindFlag,
		DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, const unsigned int& _BindFlag);
};

