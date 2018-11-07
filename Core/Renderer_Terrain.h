#pragma once
#include "Renderer.h"

class TERRAIN_FD
{
public:
	int FloorCnt;
	int SizeX;
	int SizeZ;
	int temp;
	int IsBump[4];
};

class Renderer_Terrain : public Renderer
{
private:
	TERRAIN_FD m_TFD;

public:
	void Create_Terrain(const KUINT& _X, const KUINT& _Z);
	void Render(KPtr<Camera> _Cam) override;

	void base_texture(const wchar_t* _Diff, const wchar_t* _Bump);
	void cover_texture(const wchar_t* _Diff, const wchar_t* _Bump);

public:
	Renderer_Terrain();
	~Renderer_Terrain();
};

