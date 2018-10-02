#pragma once
#include "Renderer.h"
#include "KImage.h"

class Renderer_Mesh :
	public Renderer
{
public:
	Renderer_Mesh();
	~Renderer_Mesh();


public:
	void Render(KPtr<Camera> _Camera) override;
	bool Init(int _Order = 0) override;
};

