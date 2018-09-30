#include "IsoMap_Renderer.h"

#include <ResourceManager.h>
#include <KMacro.h>
#include <KWindow.h>

#include <DirectXHeader_DE.h>

IsoMap_Renderer::IsoMap_Renderer() :m_Color(KColor::White)
{
}


IsoMap_Renderer::~IsoMap_Renderer()
{
}

bool IsoMap_Renderer::Init(int _Order)
{
	Renderer::Init(_Order);

	if (false == Set_Material(L"IMGMAT"))
	{
		return false;
	}
	if (false == Set_Mesh(L"TEXMESH"))
	{
		return false;
	}

	return true;
}


bool IsoMap_Renderer::Init(const wchar_t* _Name, const KVector2& _Size, int _Order)
{	
	image(_Name);
	if (nullptr == m_Image)
	{
		return false;
	}
	if (false == Init(_Order))
	{
		return false;
	}

	m_TileSize.m_Front = _Size;
	m_TileSize.z = 1.0f;

	M_ScaleMat.Scale(m_TileSize);

	return true;
}

void IsoMap_Renderer::image(const wchar_t* _Name)
{
	if (nullptr == _Name)
	{
		m_Image == nullptr;
		return;
	}

	m_Image = ResourceManager<KImage>::Find(_Name);
	KASSERT(nullptr == m_Image);
}


void IsoMap_Renderer::Render(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	if (nullptr == m_Image)
	{
		return;
	}

	Indepen_Update();
	
	S_IsoTile = m_IsoTileMap.begin();
	E_IsoTile = m_IsoTileMap.end();

	KVector2 Index;

	for (; S_IsoTile != E_IsoTile; ++S_IsoTile)
	{
		Index = S_IsoTile->first;

		m_TilePos.x = (Index.ix - Index.iy) * m_TileSize.x_part();
		m_TilePos.y = (Index.ix + Index.iy) * -m_TileSize.y_part();

		M_PosMat.Translation(m_TilePos);

		KMatrix Mat = M_ScaleMat * M_PosMat * independent_Matrix();
		Mat = Mat * _Camera->View_Proj();


		// 넘기기전 보더세팅 - 스텍쓰기로 한다.
		KBorder NewBorder;
		NewBorder.m_Color = this->m_Color;
		NewBorder.m_UvPerPixel = KVector2(1.0f / m_Image->size().x, 1.0f / m_Image->size().y);
		NewBorder.m_Border = KVector2::Zero;

		m_Material->VShader()->Set_ConstBuffer<KMatrix>(L"TRANS", Mat.TransPose_Referance());
		m_Material->PShader()->Set_ConstBuffer<KBorder>(L"BORDER", NewBorder);
		m_Material->PShader()->Set_ConstBuffer<KVector4>(L"MULTI", m_Image->uv(S_IsoTile->second->m_Index));

		m_Material->Update();
		m_Image->sampler()->Update();
		m_Image->texture()->Update();
		m_Mesh->Update();
		m_Mesh->Render();
	}
}

KPtr<IsoMap_Renderer::KTile> IsoMap_Renderer::find_tile(const KVector2& _Tile)
{
	return Map_Find<KPtr<IsoMap_Renderer::KTile>>(m_IsoTileMap, _Tile);
}
void IsoMap_Renderer::create_tile(KVector2 _Pos, const int& _index)
{

	// 변환 작업이 필요하다.

	_Pos.x += m_TileSize.x_part();
	_Pos.y -= m_TileSize.y_part();

	KVector2 Index;

	Index.ix = (int)(((_Pos.x / m_TileSize.x_part() + _Pos.y / -m_TileSize.y_part()) / 2) + 0.5f);
	Index.iy = (int)(((_Pos.y / -m_TileSize.y_part() + _Pos.x / -m_TileSize.x_part()) / 2) + 0.5f);
	
	//// 아무런 생성자도 만들지 않았을때  {}안에 그대로 맴버변수 순서대로만 넣어줘도 된다.
	//m_TileMap.insert(std::unordered_map<__int64, HPTR<HTile>>::value_type(_Tile, new HTile( _Index )));
	create_tile(Index.ix, Index.iy, _index);
}

void IsoMap_Renderer::create_tile(int _x, int _y, int _index/* = 0*/)
{
	KVector2 Key;

	Key.ix = _x;
	Key.iy = _y;

	KPtr<KTile> Tile = find_tile(Key);

	if (nullptr != Tile)
	{
		Tile->m_Index = _index;
		return;
	}

	Tile = new KTile(_index);

	// 아무런 생성자도 만들지 않았을때  {}안에 그대로 맴버변수 순서대로만 넣어줘도 된다.
	m_IsoTileMap.insert(std::unordered_map<__int64, KPtr<KTile>>::value_type(Key, Tile));

}