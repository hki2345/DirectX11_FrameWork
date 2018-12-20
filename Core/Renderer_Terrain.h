#pragma once
#include "Renderer.h"
#include "DXContainer_DE.h"

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
	KVector		m_MPos;

	std::vector<VTX3D>	m_TempVtx;
	std::vector<UINT>	m_TempIdx;
	KPtr<Texture>		m_NormalTex;


private:
	void TerrainTo_MPos(KPtr<Camera> _Camera);
	void Set_DI();
	void Ascent_Normal(const float& _Value);

public:	
	void Reset_Terrain(const KUINT& _X, const KUINT& _Z, const wchar_t* _NorMap = nullptr, const float& _HRatio = 1.0f);
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

	int& split_X()
	{
		return m_TFD.SizeX;
	}
	int& split_Z()
	{
		return m_TFD.SizeX;
	}
	void split_X(const int& _Value)
	{
		m_TFD.SizeX = _Value;
		Reset_Terrain(m_TFD.SizeX, m_TFD.SizeZ);
	}
	void split_Z(const int& _Value)
	{
		m_TFD.SizeZ = _Value;
		Reset_Terrain(m_TFD.SizeX, m_TFD.SizeZ);
	}

	void Clear();
	void Save();
	void Load(const wchar_t* _Name);

public:
	Renderer_Terrain();
	~Renderer_Terrain();
};

