#include "Renderer_Terrain.h"
#include "KMesh.h"
#include "DXContainer_DE.h"



Renderer_Terrain::Renderer_Terrain()
{
}


Renderer_Terrain::~Renderer_Terrain()
{
}



void Renderer_Terrain::Create_Terrain(const KUINT& _X, const KUINT& _Z)
{
	m_SizeX = _X;
	m_SizeZ = _Z;

	if (m_SizeX <= 0 || m_SizeZ <= 0)
	{
		return;
	}

	KPtr<KMesh> NMesh = new KMesh();

	std::vector<VTX3D> TV;
	std::vector<WORD> TI;

	VTX3D TempV;

	for (KUINT z = 0; z < m_SizeZ + 1; z++)
	{
		for (KUINT x = 0; x < m_SizeX + 1; x++)
		{
			TempV.Pos = KVector(x, .0f, z, 1.0f);
			TempV.Uv = KVector2((int)x, m_SizeZ - z);
			TempV.Color = KVector(1.0f, 1.0f, 1.0f, 1.0f);
			TempV.Normal = KVector(.0f, 1.0f, .0f, .0f);
			TempV.Tangent = KVector(1.0f, .0f, .0f, .0f);
			TempV.BNormal = KVector(.0f, .0f, 1.0f, .0f);

			TV.push_back(TempV);
		}
	}



	for (KUINT z = 0; z < m_SizeZ; z++)
	{
		for (KUINT x = 0; x < m_SizeX; x++)
		{
			TI.push_back((m_SizeX + 1) * (z + 1) + (x));
			TI.push_back((m_SizeX + 1) * (z) + (x + 1));
			TI.push_back((m_SizeX + 1) * (z) + (x));


			TI.push_back((m_SizeX + 1) * (z) + (x + 1));
			TI.push_back((m_SizeX + 1) * (z + 1) + (x));
			TI.push_back((m_SizeX + 1) * (z + 1) + (x + 1));
		}
	}

	NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	NMesh->Create_Vertex((KUINT)TV.size(), sizeof(VTX3D), D3D11_USAGE_DYNAMIC, &TV[0]);
	NMesh->Create_Index((KUINT)TI.size(), IDX32::MemberSize(), D3D11_USAGE_DEFAULT,IDX32::FM(), &TI[0]);

	Set_Mesh(NMesh);
	Set_Material(L"DEFFERD3DMAT");
}

void Renderer_Terrain::Render(KPtr<Camera> _Cam)
{

}