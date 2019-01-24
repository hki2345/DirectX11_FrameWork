#include "Renderer_Terrain.h"


#include "KMesh.h"

#include "KRay3D.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "Core_Class.h"
#include "InputManager.h"

#include "WriteStream.h"	
#include "ReadStream.h"	


Renderer_Terrain::Renderer_Terrain() :
	m_BSize(.0f),
	m_Edit(false),
	m_CalMPos(false)
{
}


Renderer_Terrain::~Renderer_Terrain()
{
}


void Renderer_Terrain::TerrainTo_MPos(KPtr<Camera> _Camera)
{
	if (false == InputManager::Check_InScr() || false == m_CalMPos)
	{
		Reset_DI();
		return;
	}


	m_CRay = _Camera->Get_Component<KRay3D>();
	if (nullptr == m_CRay)
	{
		return;
	}


	
	bool Ter = Check_Plain();
	if (false == Ter)
	{
		Reset_DI();
		return;
	}


	for (int Z = 0; Z < m_TFD.SizeZ; Z++)
	{
		for (int X = 0; X < m_TFD.SizeX; X++)
		{
			// 버텍스 평행이동
			// 브러쉬 사이즈가 잘 안먹힘 ㅡ.ㅡ;;
			// 브러쉬 사이즈  수정시 UV자체가 바뀜
			// 하지만 마법의 숫자 10.0을 대입하니 정확한 크기에 마우스좌표가 찍힘
			// 지금은 아니지만 나중에 열어보면 아마 다시 써야하는게 맞겟다. 아니면 디버그하든가.
			// 아므ㅜ래도 디버그보단 그때 새마음 새뜻으로 다시 시작하는게 더 빠르겠다.
			// LU
			KVector V0 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + X].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			// RU
			KVector V1 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			// LD
			KVector V2 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			// RD
			KVector V3 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

			Ter = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V3, V1, V0, m_CRay->ray_container()->Dist);
			if (false == Ter)
			{
				Ter = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V2, V3, V1, m_CRay->ray_container()->Dist);
			}

			if (true == Ter)
			{
				m_MPos = KMath::Calc_ColPoint(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, m_CRay->ray_container()->Dist);
				m_DI.MUv = KMath::PostoUV2_XZ(m_MPos, m_Trans, KVector((float)m_TFD.SizeX, .0f, (float)m_TFD.SizeZ, .0f));
				m_DI.MUv.y = 1.0f - m_DI.MUv.y;

				return;
			}
		}
	}


	return;
}

void Renderer_Terrain::Reset_DI()
{
	m_MPos = .0f;
	m_DI.MUv = KVector2(-1.0f, -1.0f);
}

void Renderer_Terrain::Set_DI()
{
	m_DI.BSize = m_BSize;
	m_DI.PPUV = KVector2(
		1.0f / (m_Trans->scale_local().x * m_TFD.SizeX),
		1.0f / (m_Trans->scale_local().z * m_TFD.SizeZ)
	);
}


void Renderer_Terrain::Ascent_Normal(const float& _Value)
{
	if (false == m_Edit)
	{
		return;
	}

	int X = (int)(((m_MPos.x - m_Trans->scale_local().x * .5f - m_Trans->pos_local().x) / Trans()->scale_local().x));
	int Z = (int)(((m_MPos.z + m_Trans->scale_local().z * .5f - m_Trans->pos_local().z) / Trans()->scale_local().z));


	if (nullptr == mesh())
	{
		return;
	}

	if (X < 0 || Z < 0 || m_TFD.SizeX < X || m_TFD.SizeZ < Z)
	{
		return;
	}


	m_TempVtx[(int)((Z) * (m_TFD.SizeZ + 1) + (X + 1))].Pos.y += 5.0f *DELTATIME * _Value;
	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
}

bool Renderer_Terrain::Check_Plain()
{
	bool m_OnTer = false;
	if (nullptr == m_CRay)
	{
		return m_OnTer;
	}


	// 1차 플레인 검열
	// LU
	KVector V0 = m_TempVtx[(m_TFD.SizeZ + 1) * (m_TFD.SizeX) - 1].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// RU
	KVector V1 = m_TempVtx[(m_TFD.SizeZ) * (m_TFD.SizeX) + m_TFD.SizeX].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// LD
	KVector V2 = m_TempVtx[0].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// RD
	KVector V3 = m_TempVtx[m_TFD.SizeX].Pos * m_Trans->scale_world() + m_Trans->pos_world();
	m_OnTer = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V3, V1, V0, m_CRay->ray_container()->Dist);
	if (false == m_OnTer)
	{
		m_OnTer = DirectX::TriangleTests::Intersects(m_CRay->ray_container()->Ori, m_CRay->ray_container()->Dir, V2, V3, V1, m_CRay->ray_container()->Dist);
	}

	return m_OnTer;
}


void Renderer_Terrain::Reset_Terrain(const KUINT& _X, const KUINT& _Z, const wchar_t* _NorMap /*= nullptr*/, const float& _HRatio /*= 1.0f*/)
{
	if (m_TFD.SizeX <= 0 || m_TFD.SizeZ <= 0)
	{
		return;
	}
	if (nullptr == mesh())
	{
		return;
	}

	m_TempVtx.clear();
	m_TempIdx.clear();

	VTX3D TempV;

	// 땅의 정보를 담은 노멀맵을 찾는다.
	KPtr<Texture> NTex = nullptr;
	if (nullptr != _NorMap)
	{
		NTex = ResourceManager<Texture>::Find(_NorMap);
		if (nullptr == NTex)
		{
			BBY;
		}
	}

	for (int z = 0; z < m_TFD.SizeZ + 1; z++)
	{
		for (int x = 0; x < m_TFD.SizeX + 1; x++)
		{
			// 지형을 실제로 높이는 단계
			if (nullptr != NTex)
			{
				int WX = (int)NTex->Width() / m_TFD.SizeX;
				int WZ = (int)NTex->Height() / m_TFD.SizeZ;

				KColor GCol = NTex->GetPixelF(x* WX, (int)NTex->Height() - (z * WZ));

				// 버퍼에 넘길 버텍스 정보 자체를 수정한다.
				TempV.Pos = KVector((float)x, GCol.x * _HRatio, (float)z, 1.0f);
			}
			else
			{
				TempV.Pos = KVector((float)x, .0f, (float)z, 1.0f);
			}

			TempV.Pos.w = 1.0f;
			TempV.Uv = KVector2((float)x, (float)(m_TFD.SizeZ - z));
			TempV.Color = KVector(1.0f, 1.0f, 1.0f, 1.0f);
			TempV.Normal = KVector(.0f, 1.0f, .0f, .0f);
			TempV.Tangent = KVector(1.0f, .0f, .0f, .0f);
			TempV.BNormal = KVector(.0f, .0f, -1.0f, .0f);

			m_TempVtx.push_back(TempV);
		}
	}

	for (int z = 0; z < m_TFD.SizeZ; z++)
	{
		for (int x = 0; x < m_TFD.SizeX; x++)
		{
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z)+(x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z)+(x));


			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z)+(x + 1));
		}
	}


	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
	mesh()->Update_Index((KUINT)m_TempIdx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::FM(), &m_TempIdx[0]);
}


void Renderer_Terrain::Create_Terrain(const KUINT& _X, const KUINT& _Z, const wchar_t* _NorMap /*= nullptr*/, const float& _HRatio /*= 1.0f*/)
{
	m_TFD.TexCnt = 0;
	m_TFD.SizeX = _X;
	m_TFD.SizeZ = _Z;

	if (m_TFD.SizeX <= 0 || m_TFD.SizeZ <= 0)
	{
		return;
	}

	KPtr<KMesh> NMesh = new KMesh();

	VTX3D TempV;
	

	// 땅의 정보를 담은 노멀맵을 찾는다.
	KPtr<Texture> NTex = nullptr;
	if (nullptr != _NorMap)
	{
		NTex = ResourceManager<Texture>::Find(_NorMap);
		if (nullptr == NTex)
		{
			BBY;
		}
	}




	for (int z = 0; z < m_TFD.SizeZ + 1; z++)
	{
		for (int x = 0; x < m_TFD.SizeX + 1; x++)
		{
			// 지형을 실제로 높이는 단계
			if (nullptr != NTex)
			{
				// 텍스쳐가 64 X 64 식으롤 반복 적으로 덮혀있을 떄를 대비해
				// 텍스쳐의 전체 버텍스를 맞춰주기 위함이다.
				int WX = (int)NTex->Width() / m_TFD.SizeX;
				int WZ = (int)NTex->Height() / m_TFD.SizeZ;
				

				// Z를 전체 크기에서 빼주는 이유는
				// 현재 반복해서 텍스쳐를 입히는 방식이 인덱스가 증가할 수록
				// 텍스쳐를 입히기 때문
				// 하지만 좌표로 치면 안으로 들어갈수록 - 이기 때문에
				// 부호보정이 필요하다.ㄴ
				KColor GCol = NTex->GetPixelF(x* WX, (int)NTex->Height() - (z * WZ));
				
				// 버퍼에 넘길 버텍스 정보 자체를 수정한다.
				TempV.Pos = KVector((float)x, GCol.x * _HRatio, (float)z, 1.0f);
			}
			else
			{
				TempV.Pos = KVector((float)x, .0f, (float)z, 1.0f);
			}

			TempV.Pos.w = 1.0f;
			TempV.Uv = KVector2((float)x, (float)(m_TFD.SizeZ - z));
			TempV.Color = KVector(1.0f, 1.0f, 1.0f, 1.0f);
			TempV.Normal = KVector(.0f, 1.0f, .0f, .0f);
			TempV.Tangent = KVector(1.0f, .0f, .0f, .0f);
			TempV.BNormal = KVector(.0f, .0f, -1.0f, .0f);

			m_TempVtx.push_back(TempV);
		}
	}



	for (int z = 0; z < m_TFD.SizeZ; z++)
	{
		for (int x = 0; x < m_TFD.SizeX; x++)
		{
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z) + (x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z) + (x));


			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z + 1) + (x + 1));
			m_TempIdx.push_back((m_TFD.SizeX + 1) * (z) + (x + 1));
		}
	}

	NMesh->Create_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
	NMesh->Create_Index((KUINT)m_TempIdx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::FM(), &m_TempIdx[0]);
	
	// 기본 세팅
	// NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


	Set_Mesh(NMesh);

	// 기본 세팅
	// Set_Material(L"DEFFERDTERRAINMAT");
	Set_Material(L"DTESSLEMAT");
}

void Renderer_Terrain::RenderBegin(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	TerrainTo_MPos(_Camera);
	Set_DI();

	m_DI.OnClick = .0f;

	if (true == InputManager::Check_InScr())
	{
		if (true == KEY_PRESS(L"LB"))
		{
			Ascent_Normal(1.0f);
			m_DI.OnClick = 1.0f;
		}

		if (true == KEY_PRESS(L"RB"))
		{
			Ascent_Normal(-1.0f);
			m_DI.OnClick = -1.0f;
		}
	}

	material()->PShader()->SettingCB<TERRAIN_FD>(L"TERRAIN_FD", m_TFD);
	material()->PShader()->SettingCB<DRAW_INFO>(L"DRAW_INFO", m_DI);
}

void Renderer_Terrain::base_texture(const wchar_t* _MTex)
{
	// 섞여질 텍스트 어레이 타입 -> 멀티텍스트, 텍스트 배열들은,,,
	// 8번째 이후 부터 입력된다.
	material()->Insert_TexData(TEX_TYPE::TEX_MULTI, m_TFD.TexCnt + 8, _MTex, m_TFD.TexCnt + 8, L"TerrainSmp");
	++m_TFD.TexCnt;
}


void Renderer_Terrain::Insert_CoverTex(const wchar_t* _MTex, const wchar_t* _Cover)
{
	if (m_TFD.TexCnt <= 0)
	{
		BBY;
	}

	// 기본형은 컬러로 맨처음 쫙 깔아주는 과정
	material()->Insert_TexData(TEX_TYPE::TEX_COLOR, m_TFD.FloorCnt, _Cover, m_TFD.FloorCnt);
	material()->Insert_TexData(TEX_TYPE::TEX_MULTI, m_TFD.TexCnt + 8, _MTex, m_TFD.TexCnt + 8, L"TerrainSmp");

	++m_TFD.FloorCnt;
	++m_TFD.TexCnt;
}



// 여기서 산출된 좌표로 캐릭ㅌ가 움직일 것이다.
// 따라서 모든 유닛은 이 지형을 알아야하며
// 매번 해당 좌표에 위치했을 시 그 높이를 산출해야한다.
float Renderer_Terrain::Y_Terrain(const KVector& _Pos)
{
	int X = (int)((_Pos.x - m_Trans->pos_local().x) / Trans()->scale_local().x);
	int Z = (int)((_Pos.z - m_Trans->pos_local().z) / Trans()->scale_local().z);
	
	if (X < 0 || Z < 0 || m_TFD.SizeX < X || m_TFD.SizeZ < Z)
	{
		return .0f;
	}

	// 좌상단
	KVector V0 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + X].Pos  * m_Trans->scale_world() + m_Trans->pos_world();

	// 우상단
	KVector V1 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// 좌하단
	KVector V2 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// 우하단
	KVector V3 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();


	// 검출될 y좌표
	float F0 = .0f;
	float F1 = .0f;

	KVector ObjPos = KVector(
		_Pos.x 
		, .0f
		, _Pos.z );

	DirectX::TriangleTests::Intersects(ObjPos, KVector::Up, V2, V1, V0, F0);
	DirectX::TriangleTests::Intersects(ObjPos, KVector::Up, V2, V3, V1, F1);
	
	if (0 != F0|| 0 != F1)
	{
		if (0 != F0)
		{
			return F0 * Trans()->scale_local().y * .5f;
		}
		else if (0 != F1)
		{
			return F1 * Trans()->scale_local().y * .5f;
		}
	}

	return 0.0f;
}

void Renderer_Terrain::Clear()
{
	for (size_t i = 0; i < m_TempVtx.size(); i++)
	{
		m_TempVtx[i].Pos.y = .0f;

	}
	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
}

void Renderer_Terrain::Save()
{
	std::wstring Path = PathManager::Find_ForderPath(L"KTD");
	Path += name();
	Path += L".KTD";

	WriteStream WS = WriteStream(Path.c_str());


	int VT = (int)m_TempVtx.size();
	WS.Write(VT);
	for (int i = 0; i < VT; i++)
	{
		WS.Write(m_TempVtx[i].BNormal);
		WS.Write(m_TempVtx[i].Color);
		WS.Write(m_TempVtx[i].Normal);
		WS.Write(m_TempVtx[i].Pos);
		WS.Write(m_TempVtx[i].Tangent);
		WS.Write(m_TempVtx[i].Uv);
	}

	int IT = (int)m_TempIdx.size();
	WS.Write(IT);
	for (int i = 0; i < IT; i++)
	{
		WS.Write(m_TempIdx[i]);
	}
}

void Renderer_Terrain::Load(const wchar_t* _Name)
{
	if (nullptr == mesh())
	{
		Set_Mesh(new KMesh());
	}

	m_TempVtx.clear();
	m_TempIdx.clear();


	std::wstring Path = PathManager::Find_ForderPath(L"KTD");
	Path += _Name;
	Path += L".KTD";

	ReadStream RS = ReadStream(Path.c_str());


	int VT;
	RS.Read(VT);
	for (int i = 0; i < VT; i++)
	{
		VTX3D TT;

		RS.Read(TT.BNormal);
		RS.Read(TT.Color);
		RS.Read(TT.Normal);
		RS.Read(TT.Pos);
		RS.Read(TT.Tangent);
		RS.Read(TT.Uv);

		m_TempVtx.push_back(TT);
	}

	int IT = (int)m_TempIdx.size();
	RS.Read(IT);
	for (int i = 0; i < IT; i++)
	{
		UINT TT;
		RS.Read(TT);
		m_TempIdx.push_back(TT);
	}


	mesh()->Update_Vertex((KUINT)m_TempVtx.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &m_TempVtx[0]);
	mesh()->Update_Index((KUINT)m_TempIdx.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::FM(), &m_TempIdx[0]);
}



float Renderer_Terrain::TER_Left()
{
	return one()->Trans()->pos_world().x;
}

float Renderer_Terrain::TER_Right()
{
	return one()->Trans()->pos_world().x + m_TFD.SizeX * one()->Trans()->scale_world().x;
}

float Renderer_Terrain::TER_Up()
{
	return one()->Trans()->pos_world().z + m_TFD.SizeZ * one()->Trans()->scale_world().z;
}

float Renderer_Terrain::TER_Down()
{
	return one()->Trans()->pos_world().z;
}