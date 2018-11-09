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
	std::vector<KVector> m_PosVtx;

public:
	void Create_Terrain(const KUINT& _X, const KUINT& _Z, const wchar_t* _NorMap = nullptr, const float& _HRatio = 1.0f);
	void Render(KPtr<Camera> _Cam) override;

	void base_texture(const wchar_t* _MTex);
	void Insert_CoverTex(const wchar_t* _MTex, const wchar_t* _Cover);

	// 해당 좌ㅏ표에 위치한 Y축 좌표를 반환해 준다.
	float Y_Terrain(const KVector& _Pos);

public:
	Renderer_Terrain();
	~Renderer_Terrain();
};

