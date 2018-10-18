#pragma once
#include "HTexture.h"
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
class HRenderTaget : public HRes
{
private:
	KPtr<HTexture> m_Tex;
	HVEC		   m_Color;

public:
	KPtr<HTexture> TagetTex() 
	{
		return m_Tex;
	}

public:
	bool Create(const KUINT& _W, const KUINT& _H, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(const KUINT& _W, const KUINT& _H, HVEC _Color, const KUINT& _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag);

	void Clear();

public:
	HRenderTaget();
	~HRenderTaget();
};

