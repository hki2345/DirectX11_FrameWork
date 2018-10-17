#pragma once
#include "HTexture.h"

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
	HPTR<HTexture> m_Tex;
	HVEC		   m_Color;

public:
	HPTR<HTexture> TagetTex() 
	{
		return m_Tex;
	}

public:
	bool Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _pTex2D, UINT _BindFlag);

	void Clear();

public:
	HRenderTaget();
	~HRenderTaget();
};
