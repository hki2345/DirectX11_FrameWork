#pragma once
#include <Renderer.h>
#include <KImage.h>
#include <unordered_map>


class IsoMap_Renderer : public Renderer
{
public:
	IsoMap_Renderer();
	~IsoMap_Renderer();

private:
	KPtr<KImage>	m_Image;
	KColor			m_Color;

	KMatrix M_PosMat;
	KMatrix M_ScaleMat;
	KMatrix M_Mat;

	KVector2 m_Index;
	KVector4 m_TilePos;
	KVector4 m_TileSize;

public:
	class KTile : public SmartPtr
	{
	public:
		int m_Index;

	public:
		bool operator == (const int& _value)
		{
			return m_Index == _value;
		}

	public:
		KTile(int _Value) :m_Index(_Value)
		{
			int a = 0;
		}
	};

private:
	std::unordered_map<__int64, KPtr<KTile>>::iterator S_IsoTile;
	std::unordered_map<__int64, KPtr<KTile>>::iterator E_IsoTile;
	std::unordered_map<__int64, KPtr<KTile>> m_IsoTileMap;

public:
	KPtr<KTile> find_tile(const KVector2& _Tile);

	// 마우스 포지션에 따른 생성
	void create_tile(KVector2 _Pos, const int& _index = 0);
	void create_tile(int _x, int _y, int _index = 0);

public:
	void image(const wchar_t* _Name);
	KPtr<KImage> image() { return m_Image; }

	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _Name, const KVector2& _Size, int _Order = 0);

	void Render(KPtr<Camera> _Camera) override;
};

