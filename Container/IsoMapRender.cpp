#include "IsoMapRender.h"
#include <HResMgr.h>
#include <KMacro.h>
#include <HWindow.h>


IsoMapRender::IsoMapRender() : m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
}


IsoMapRender::~IsoMapRender()
{
}


bool IsoMapRender::Init(int _Order)
{
	HRenderer::Init(_Order);

	if (false == SetMat(L"IMGMAT"))
	{
		return false;
	}
	if (false == SetMesh(L"TEXMESH"))
	{
		return false;
	}

	return true;
}

bool IsoMapRender::Init(const wchar_t* _ImgName, HVEC2 _Size, int _Order)
{
	Image(_ImgName);
	if (nullptr == m_Img)
	{
		return false;
	}

	if (false == Init(_Order))
	{
		return false;
	}

	m_TileSIze.x = _Size.x;
	m_TileSIze.y = _Size.y;
	m_TileSIze.z = 1.0f;

	TileSizeMat.Scale(m_TileSIze);
	return true;
}

void IsoMapRender::Image(const wchar_t* _ImageName)
{
	if (nullptr == _ImageName)
	{
		m_Img = nullptr;
		return;
	}

	m_Img = HResMgr<HImage>::Find(_ImageName);
	KASSERT(nullptr == m_Img);
}

void IsoMapRender::Render(KPtr<HCamera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	if (nullptr == m_Img)
	{
		return;
	}
	// 버텍스 쉐이더가 

	SubTransUpdate();

	CSWMat();

	StartIter =	m_TileMap.begin();
	EndIter = m_TileMap.end();

	HVEC2 Index;

	for (; StartIter != EndIter; ++StartIter)
	{
		Index = StartIter->first;

		TilePos.x = (Index.ix - Index.iy) * m_TileSIze.HX();
		TilePos.y = (Index.ix + Index.iy) * -m_TileSIze.HY();

		TilePosMat.Trans(TilePos);

		HMAT Mat = TileSizeMat * TilePosMat * CSWMat();
		Mat = Mat * _Camera->VP();

		m_Mat->VTXSH()->SettingCB<HMAT>(L"TRANS", Mat.RTranspose());
		m_Mat->PIXSH()->SettingCB<HVEC>(L"MULCOLOR", m_Color);
		m_Mat->PIXSH()->SettingCB<HVEC>(L"IMGUV", m_Img->Uv(StartIter->second->Index));

		m_Mat->Update();
		m_Img->GetSam()->Update(0);
		m_Img->GetTex()->Update(0);
		m_Mesh->Update();
		m_Mesh->Render();
	}

}

KPtr<IsoMapRender::HTile> IsoMapRender::FindTile(HVEC2 _Tile)
{
	return Map_Find<KPtr<IsoMapRender::HTile>>(m_TileMap, _Tile);
}

HVEC2 IsoMapRender::WorldToIndex(HVEC2 _WorldPos) 
{
	float X = ((_WorldPos.x / m_TileSIze.HX() + _WorldPos.y / -m_TileSIze.HY()) / 2);
	float Y = ((_WorldPos.y / -m_TileSIze.HY() + _WorldPos.x / -m_TileSIze.HX()) / 2);

	if (0.5f <= X) { X += 0.5f; }
	if (-0.5f >= X) { X -= 0.5f; }
	if (0.5f <= Y) { Y += 0.5f; }
	if (-0.5f >= Y) { Y -= 0.5f; }

	return{ (int)X, (int)Y };
}

void IsoMapRender::CreateTile(HVEC2 _WorldPos, int _Index /*= 0*/)
{
	// 변환 작업이 필요하다.

	HVEC2 Index = WorldToIndex(_WorldPos);
	//// 아무런 생성자도 만들지 않았을때  {}안에 그대로 맴버변수 순서대로만 넣어줘도 된다.
	//m_TileMap.insert(std::unordered_map<__int64, KPtr<HTile>>::value_type(_Tile, new HTile( _Index )));
	CreateTile(Index.ix, Index.iy, _Index);
}

void IsoMapRender::CreateTile(int _x, int _y, int _Index) {

	HVEC2 Key;

	Key.ix = _x;
	Key.iy = _y;

	KPtr<HTile> Tile = FindTile(Key);

	if (nullptr != Tile)
	{
		Tile->Index = _Index;
		return;
	}

	// 아무런 생성자도 만들지 않았을때  {}안에 그대로 맴버변수 순서대로만 넣어줘도 된다.
	m_TileMap.insert(std::unordered_map<__int64, KPtr<HTile>>::value_type(Key, new HTile(_Index)));

}