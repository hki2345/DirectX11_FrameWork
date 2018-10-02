#pragma once
#include "DirectXHeader.h"
#include "KResource.h"

class Blend : public KResource
{
private:
	ID3D11BlendState*	m_BlendState;
	D3D11_BLEND_DESC	m_Desc;
	KColor				m_Color;

public:
	bool Create();
	void Update();

public:
	Blend();
	~Blend();
};

