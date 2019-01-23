#pragma once
#include "Renderer.h"


class Renderer_UI : public Renderer
{
public:
	Renderer_UI();
	~Renderer_UI();

public:
	void Update_Trans(KPtr<Camera> _Camera) override;
	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data) override;

public:
	bool Init(int _Order = 0) override;
	// bool Init(const wchar_t* _ImgName, int _Order = 0);

};

