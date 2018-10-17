#pragma once
#include "DHeader.h"
#include "HRes.h"

class HBlend : public HRes
{
private:
	ID3D11BlendState* m_pBlendState;
	D3D11_BLEND_DESC BDesc;
	HCOLOR			 m_Color;

public:
	bool Create();
	bool Create(D3D11_BLEND_DESC _Decs);
	bool Update();

public:
	HBlend();
	~HBlend();
};

