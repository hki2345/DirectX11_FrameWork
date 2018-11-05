#pragma once
#include "Renderer.h"
class Renderer_Terrain : public Renderer
{
private:
	KUINT m_SizeX;
	KUINT m_SizeZ;

public:
	void Create_Terrain(const KUINT& _X, const KUINT& _Z);
	void Render(KPtr<Camera> _Cam) override;

public:
	Renderer_Terrain();
	~Renderer_Terrain();
};

