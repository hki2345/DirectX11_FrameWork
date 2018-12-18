#include "Renderer_Terrain.h"
#include "KMesh.h"
#include "DXContainer_DE.h"

#include "KRay3D.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "Core_Class.h"
#include "InputManager.h"


Renderer_Terrain::Renderer_Terrain() :
	m_BSize(.0f)
{
}


Renderer_Terrain::~Renderer_Terrain()
{
}


KVector Renderer_Terrain::TerrainTo_MPos()
{
	KPtr<KRay3D> CRay =  state()->Camera()->Get_Component<KRay3D>();
	if (nullptr == CRay)
	{
		return KVector::Zero;
	}

	bool TBol = false;

	for (size_t Z = 0; Z < m_TFD.SizeZ; Z++)
	{
		for (size_t X = 0; X < m_TFD.SizeX; X++)
		{
			// LU
			KVector V0 = m_PosVtx[((Z + 1) * (m_TFD.SizeX + 1)) + X] * m_Trans->scale_world();

			// RU
			KVector V1 = m_PosVtx[((Z + 1) * (m_TFD.SizeX + 1)) + (X + 1)] * m_Trans->scale_world();

			// LD
			KVector V2 = m_PosVtx[((Z) * (m_TFD.SizeX + 1)) + (X)] * m_Trans->scale_world();

			// RD
			KVector V3 = m_PosVtx[((Z) * (m_TFD.SizeX + 1)) + (X + 1)] * m_Trans->scale_world();

			TBol = DirectX::TriangleTests::Intersects(CRay->ray_container()->Ori, CRay->ray_container()->Dir, V2, V1, V0, CRay->ray_container()->Dist);
			if (true == TBol)
			{
				KVector TT = KMath::Calc_ColPoint(CRay->ray_container()->Ori, CRay->ray_container()->Dir, CRay->ray_container()->Dist);
				m_DI.MousePos = KMath::PostoUV2_XZ(TT, m_Trans, KVector((float)m_TFD.SizeX, .0f, (float)m_TFD.SizeZ, .0f));

				m_DI.MousePos.y = 1.0f - m_DI.MousePos.y;
				KLOG(L"Terrain To MPos: %f, %f, %f", TT.x, TT.y, TT.z);
				KLOG(L"Terrain To MPos UV: %f, %f", m_DI.MousePos.x, m_DI.MousePos.y);
				KLOG(L"%b", TBol);

				return TT;
			}

			TBol = DirectX::TriangleTests::Intersects(CRay->ray_container()->Ori, CRay->ray_container()->Dir, V2, V3, V1, CRay->ray_container()->Dist);
			if (true == TBol)
			{
				KVector TT = KMath::Calc_ColPoint(CRay->ray_container()->Ori, CRay->ray_container()->Dir, CRay->ray_container()->Dist);
				m_DI.MousePos = KMath::PostoUV2_XZ(TT, m_Trans, KVector((float)m_TFD.SizeX, .0f, (float)m_TFD.SizeZ, .0f));

				m_DI.MousePos.y = 1.0f - m_DI.MousePos.y;

				KLOG(L"Terrain To MPos: %f, %f, %f", TT.x, TT.y, TT.z);
				KLOG(L"Terrain To MPos UV: %f, %f", m_DI.MousePos.x, m_DI.MousePos.y);
				KLOG(L"%b", TBol);

				return TT;
			}
		}
	}


	return KVector::Zero;
}

void Renderer_Terrain::Set_DI()
{
	m_DI.BSize = m_BSize;
	m_DI.PPUV = KVector2(
		1.0f / (m_Trans->scale_local().x * m_TFD.SizeX),
		1.0f / (m_Trans->scale_local().z * m_TFD.SizeZ)
	);

	if (true == KEY_PRESS("MouseButton"))
	{
		m_DI.OnClick = 1.0f;
	}
	else
	{
		m_DI.OnClick = .0f;
	}
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

	std::vector<VTX3D> TV;
	std::vector<UINT> TI;

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
				m_PosVtx.push_back(TempV.Pos);
			}
			else
			{
				TempV.Pos = KVector((float)x, .0f, (float)z, 1.0f);
				m_PosVtx.push_back(TempV.Pos);
			}

			TempV.Pos.w = 1.0f;
			TempV.Uv = KVector2((float)x, (float)(m_TFD.SizeZ - z));
			TempV.Color = KVector(1.0f, 1.0f, 1.0f, 1.0f);
			TempV.Normal = KVector(.0f, 1.0f, .0f, .0f);
			TempV.Tangent = KVector(1.0f, .0f, .0f, .0f);
			TempV.BNormal = KVector(.0f, .0f, -1.0f, .0f);

			TV.push_back(TempV);
		}
	}



	for (int z = 0; z < m_TFD.SizeZ; z++)
	{
		for (int x = 0; x < m_TFD.SizeX; x++)
		{
			TI.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			TI.push_back((m_TFD.SizeX + 1) * (z) + (x + 1));
			TI.push_back((m_TFD.SizeX + 1) * (z) + (x));


			TI.push_back((m_TFD.SizeX + 1) * (z + 1) + (x));
			TI.push_back((m_TFD.SizeX + 1) * (z + 1) + (x + 1));
			TI.push_back((m_TFD.SizeX + 1) * (z) + (x + 1));
		}
	}

	NMesh->Create_Vertex((KUINT)TV.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &TV[0]);
	NMesh->Create_Index((KUINT)TI.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT, IDX32::FM(), &TI[0]);
	
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
	TerrainTo_MPos();
	Set_DI();

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
	int X = (int)(_Pos.x / Trans()->scale_local().x);
	int Z = (int)(_Pos.z / Trans()->scale_local().z);

	/*if (0 > X)
	{
		return .0f;
	}

	if (0 > Z)
	{
		return .0f;
	}
*/


	// 좌상단
	KVector V0 = m_PosVtx[((Z + 1) * (m_TFD.SizeX + 1)) + X];

	// 우상단
	KVector V1 = m_PosVtx[((Z + 1) * (m_TFD.SizeX + 1)) + (X + 1)];

	// 좌하단
	KVector V2 = m_PosVtx[((Z) * (m_TFD.SizeX + 1)) + (X)];

	// 우하단
	KVector V3 = m_PosVtx[((Z) * (m_TFD.SizeX + 1)) + (X + 1)];

	// 검출될 y좌표
	float F0 = .0f;
	float F1 = .0f;

	KVector ObjPos = KVector(
		_Pos.x / Trans()->scale_local().x
		, .0f
		, _Pos.x / Trans()->scale_local().z);

	DirectX::TriangleTests::Intersects(ObjPos, KVector::Up, V2, V1, V0, F0);
	DirectX::TriangleTests::Intersects(ObjPos, KVector::Up, V2, V3, V1, F1);

	float TempY = .0f;

	if (0 != F0|| 0 !=F1)
	{
		if (0!= F0)
		{
			return TempY = F0;
		}
		else if (0 != F1)
		{
			return TempY = F1;
		}
	}

	return TempY * Trans()->scale_local().y;
}