#pragma once
#include "Renderer.h"



class Draw_Info
{
public:
	KVector2 MousePos;
	KVector2 PPUV;
	float BSize;
	float OnClick;
	float Tmp1;
	float Tmp2;
};

class KPlane3D_Col;
class Renderer_Draw : public Renderer
{
private:
	Draw_Info	m_DI;
	KPtr<KPlane3D_Col> m_Col;

public:
	bool Init(const int& _Order = 0);

	void Update_Trans(KPtr<Camera> _Camera) override;
	void Update();

	void RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx) override;


public:
	void brush_size(const float& _Value)
	{
		m_DI.BSize = _Value;
	}

public:
	Renderer_Draw();
	~Renderer_Draw();
};

