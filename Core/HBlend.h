#pragma once
#include "DXContainer.h"
#include "Resource.h"

class HBlend : public Resource
{
private:
	ID3D11BlendState* m_pBlendState;
	D3D11_BLEND_DESC BDesc;
	KColor			 m_Color;

public:
	bool Create();
	bool Create(D3D11_BLEND_DESC _Decs);
	bool Update();

public:
	HBlend();
	~HBlend();
};

