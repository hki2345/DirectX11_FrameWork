#pragma once
#include "Renderer.h"

class TERRAIN_FD
{
public:
	int FloorCnt;
	int TexCnt;
	int SizeX;
	int SizeZ;
	int IsBump[4];
};

class Renderer_Terrain : public Renderer
{
private:
	TERRAIN_FD m_TFD;

public:
	void Create_Terrain(const KUINT& _X, const KUINT& _Z);
	void Render(KPtr<Camera> _Cam) override;

	void base_texture(const wchar_t* _MTex);
	void Insert_CoverTex(const wchar_t* _MTex, const wchar_t* _Cover);

public:
	Renderer_Terrain();
	~Renderer_Terrain();
};

