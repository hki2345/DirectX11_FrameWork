#pragma once
#include "Renderer.h"
class Renderer_Rect3D :
	public Renderer
{
public:
	Renderer_Rect3D();
	~Renderer_Rect3D();
	
public:
	void Render(KPtr<Camera> _Camera) override;
	bool Init(int _Order = 0) override;

};

