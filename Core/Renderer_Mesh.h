#pragma once
#include "Renderer.h"
#include "KImage.h"

class Renderer_Mesh :
	public Renderer
{
public:
	Renderer_Mesh();
	~Renderer_Mesh();



private:
	KPtr<KImage> m_Image;

public:
	void Render(KPtr<Camera> _Camera) override;
	void image(const wchar_t* _Name);
	bool Init(int _Order = 0) override;
};

