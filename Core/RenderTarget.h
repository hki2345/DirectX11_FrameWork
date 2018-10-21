#pragma once
#include "Texture.h"
#include "KMacro.h"

// �������� �ӷ��� �������� �����Ǵ°�?

// ù��° ��� Ÿ��Ƚ��
// �ι�° ��� ����Ƚ��
// ����° ��� ���ο��� 

// ����
// 1. �������� �ʿ��� ������ ��Ƴ��� �� Ÿ���� �ٸ����� �̿�ɼ� �ִ�.
// ����
// 1. �⺻������ ������ �ñ� ������. 1~2���� ����Ϸ��� 
// 2. ����ó���� �����. ���߿� ���� ó�� ����� �Ѵ�.

// �����꿡 �־ ���۵� ������°��� Ÿ��Ƚ���� �ø���
// ���ο����� �������� ���̴°�.

// ��Ƽ���� Ÿ����.
class RenderTarget : public Resource
{
public:
	RenderTarget();
	~RenderTarget();

private:
	KPtr<Texture> m_Tex;
	KVector4		   m_Color;

public:
	KPtr<Texture> target_tex() 
	{
		return m_Tex;
	}

	// UINT ���� unsigned int�� �տ� ������ -> UINT�� windows.h�� �ʿ���ϴµ�
	// ���� ȯ�濡 ���� ���� ���� �����ϱ�.
public:
	bool Create(const KUINT& _W, const KUINT& _H, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(const KUINT& _W, const KUINT& _H, KVector4 _Color, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag);

	void Clear();
};

