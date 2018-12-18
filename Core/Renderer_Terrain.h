#pragma once
#include "Renderer.h"

class TERRAIN_FD
{
public:
	int FloorCnt;
	int SizeX;
	int SizeZ;
	int TexCnt;
	int IsBump[4];
};


class Renderer_Terrain : public Renderer
{
private:
	float m_BSize;

	TERRAIN_FD m_TFD;
	DRAW_INFO	m_DI;
	std::vector<KVector> m_PosVtx;


private:
	KVector TerrainTo_MPos();
	void Set_DI();

public:	
	void Create_Terrain(const KUINT& _X, const KUINT& _Z, const wchar_t* _NorMap = nullptr, const float& _HRatio = 1.0f);
	void RenderBegin(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx) override;

	void base_texture(const wchar_t* _MTex);
	void Insert_CoverTex(const wchar_t* _MTex, const wchar_t* _Cover);

	// 해당 좌ㅏ표에 위치한 Y축 좌표를 반환해 준다.
	float Y_Terrain(const KVector& _Pos);
	void brush_size(const float& _size)
	{
		m_BSize = _size;
	}

	TERRAIN_FD floor_date()
	{
		return m_TFD;
	}

public:
	Renderer_Terrain();
	~Renderer_Terrain();
};

