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
			// ���ؽ� �����̵�
			// �귯�� ����� �� �ȸ��� ��.��;;
			// �귯�� ������  ������ UV��ü�� �ٲ�
			// ������ ������ ���� 10.0�� �����ϴ� ��Ȯ�� ũ�⿡ ���콺��ǥ�� ����
			// ������ �ƴ����� ���߿� ����� �Ƹ� �ٽ� ����ϴ°� �°ٴ�. �ƴϸ� ������ϵ簡.
			// �ƹǤ̷��� ����׺��� �׶� ������ �������� �ٽ� �����ϴ°� �� �����ڴ�.
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


	// 1�� �÷��� �˿�
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

	// ���� ������ ���� ��ָ��� ã�´�.
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
			// ������ ������ ���̴� �ܰ�
			if (nullptr != NTex)
			{
				int WX = (int)NTex->Width() / m_TFD.SizeX;
				int WZ = (int)NTex->Height() / m_TFD.SizeZ;

				KColor GCol = NTex->GetPixelF(x* WX, (int)NTex->Height() - (z * WZ));

				// ���ۿ� �ѱ� ���ؽ� ���� ��ü�� �����Ѵ�.
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
	

	// ���� ������ ���� ��ָ��� ã�´�.
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
			// ������ ������ ���̴� �ܰ�
			if (nullptr != NTex)
			{
				// �ؽ��İ� 64 X 64 ������ �ݺ� ������ �������� ���� �����
				// �ؽ����� ��ü ���ؽ��� �����ֱ� �����̴�.
				int WX = (int)NTex->Width() / m_TFD.SizeX;
				int WZ = (int)NTex->Height() / m_TFD.SizeZ;
				

				// Z�� ��ü ũ�⿡�� ���ִ� ������
				// ���� �ݺ��ؼ� �ؽ��ĸ� ������ ����� �ε����� ������ ����
				// �ؽ��ĸ� ������ ����
				// ������ ��ǥ�� ġ�� ������ ������ - �̱� ������
				// ��ȣ������ �ʿ��ϴ�.��
				KColor GCol = NTex->GetPixelF(x* WX, (int)NTex->Height() - (z * WZ));
				
				// ���ۿ� �ѱ� ���ؽ� ���� ��ü�� �����Ѵ�.
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
	
	// �⺻ ����
	// NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


	Set_Mesh(NMesh);

	// �⺻ ����
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
	// ������ �ؽ�Ʈ ��� Ÿ�� -> ��Ƽ�ؽ�Ʈ, �ؽ�Ʈ �迭����,,,
	// 8��° ���� ���� �Էµȴ�.
	material()->Insert_TexData(TEX_TYPE::TEX_MULTI, m_TFD.TexCnt + 8, _MTex, m_TFD.TexCnt + 8, L"TerrainSmp");
	++m_TFD.TexCnt;
}


void Renderer_Terrain::Insert_CoverTex(const wchar_t* _MTex, const wchar_t* _Cover)
{
	if (m_TFD.TexCnt <= 0)
	{
		BBY;
	}

	// �⺻���� �÷��� ��ó�� �� ����ִ� ����
	material()->Insert_TexData(TEX_TYPE::TEX_COLOR, m_TFD.FloorCnt, _Cover, m_TFD.FloorCnt);
	material()->Insert_TexData(TEX_TYPE::TEX_MULTI, m_TFD.TexCnt + 8, _MTex, m_TFD.TexCnt + 8, L"TerrainSmp");

	++m_TFD.FloorCnt;
	++m_TFD.TexCnt;
}



// ���⼭ ����� ��ǥ�� ĳ������ ������ ���̴�.
// ���� ��� ������ �� ������ �˾ƾ��ϸ�
// �Ź� �ش� ��ǥ�� ��ġ���� �� �� ���̸� �����ؾ��Ѵ�.
float Renderer_Terrain::Y_Terrain(const KVector& _Pos)
{
	int X = (int)((_Pos.x - m_Trans->pos_local().x) / Trans()->scale_local().x);
	int Z = (int)((_Pos.z - m_Trans->pos_local().z) / Trans()->scale_local().z);
	
	if (X < 0 || Z < 0 || m_TFD.SizeX < X || m_TFD.SizeZ < Z)
	{
		return .0f;
	}

	// �»��
	KVector V0 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + X].Pos  * m_Trans->scale_world() + m_Trans->pos_world();

	// ����
	KVector V1 = m_TempVtx[((Z + 1) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// ���ϴ�
	KVector V2 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X)].Pos * m_Trans->scale_world() + m_Trans->pos_world();

	// ���ϴ�
	KVector V3 = m_TempVtx[((Z) * (m_TFD.SizeX + 1)) + (X + 1)].Pos * m_Trans->scale_world() + m_Trans->pos_world();


	// ����� y��ǥ
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