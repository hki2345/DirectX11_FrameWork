#include "Renderer_BonAni.h"
#include "Texture.h"
#include "TimeManager.h"
#include "DXContainer_DE.h"
#include "ResourceManager.h"
#include "DebugManager.h"




Renderer_BonAni::Renderer_BonAni() :
m_pLoader(nullptr), 
m_ClipInx(0), 
m_FrameCnt(30), 
m_UpdateSpd(.0f), 
m_UpdateTime(.0f), 
SFrame(0), 
EFrame(0)
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_DATA;
	ROpt.IsBoneAni = 1;
}


Renderer_BonAni::~Renderer_BonAni()
{
	if (nullptr != m_pLoader)
	{
		delete m_pLoader;
	}
}



// �ϴ�, ���� �ſ� ������ �����.
void Renderer_BonAni::Load_FbxTest(const wchar_t* _Path)
{
	m_pLoader = new FBXLoader();
	m_pLoader->Load_FBX(_Path);

	KFBX* pFbx = m_pLoader->m_pNewFbx;

	for (size_t MeshInx = 0; MeshInx < pFbx->MeshData_Vec.size(); MeshInx++)
	{
		Mesh_FbxData* pMD = pFbx->MeshData_Vec[MeshInx];

		if (nullptr == pMD)
		{
			BBY;
		}

		KPtr<KMesh> NMesh = new KMesh();
		NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ���ؽ� ����
		if (false == NMesh->Create_Vertex((KUINT)pMD->VertVec.size()
		, sizeof(Vertex_FbxData)
		, D3D11_USAGE_DYNAMIC
		, &pMD->VertVec[0]))
		{
			BBY;
		}

		for (size_t SubInx = 0; SubInx < pMD->IdxVec.size(); SubInx++)
		{
			NMesh->Create_Index(
				(KUINT)pMD->IdxVec[SubInx].size()
				, IDX32::MemberSize()
				, D3D11_USAGE_DEFAULT
				, IDX32::FM()
				, &pMD->IdxVec[SubInx][0]
			);
		}


		// �ش� �޽��� ������ �ִ� ���� ����
		Set_Mesh(NMesh, (int)MeshInx);



		// ���͸��� -> ���͸��� ������ŭ �� �����־�� �Ѵ�.
		for (KUINT MtlIdx = 0; MtlIdx < (KUINT)pMD->m_MtlVec.size(); MtlIdx++)
		{
			switch (ROpt.Defferd_orForward)
			{
			case 0:
				Set_Material(L"MESH3DVTX", MtlIdx);
				break;

			case 1:
				Set_Material(L"DEFFERD3DANIMAT", MtlIdx);
				break;

			default:
				break;
			}

			KPtr<KMaterial> CMat = material(MtlIdx);

			Material_FbxData* pData = pMD->m_MtlVec[MtlIdx];

			// ��ǻ�� �޾ƿ���
			if (pData->Diff != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Diff.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, Tex->FileForder());
			}


			// ���� �޾ƿ���
			if (pData->Bump != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Bump.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, Tex->FileForder());
			}


			// ���� �޾ƿ���
			if (pData->Spec != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Spec.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_SPEC, 2, Tex->FileForder());
			}

			// ������ ������ ���͸��� ������. -> �������� �⺻ �޽��� ���� ����
			Insert_RenderData((KUINT)MeshInx, 0, MtlIdx, MtlIdx);
		}
	}

	if (0 >=  m_pLoader->m_pNewFbx->Ani_Vec.size())
	{
		return;
	}

	m_pBoneTex = new Texture();
	m_pBoneTex->Create(
		m_pLoader->m_BoneCnt * 4, 1
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT
		, D3D11_USAGE_DYNAMIC
	);

	m_MXData_CurAni.resize(m_pLoader->m_BoneCnt);
}

void Renderer_BonAni::EndUpdate()
{
	if (0 >= m_pLoader->m_pNewFbx->Ani_Vec.size())
	{
		return;
	}

	m_CurTime = .0f;

	KFBX* pFbx = m_pLoader->m_pNewFbx;

	m_UpdateTime += DELTATIME;

	if (m_UpdateTime >= pFbx->Ani_Vec[m_ClipInx]->Length_Time)
	{
		m_UpdateTime = .0f;
	}

	// ���� ���� �ð��� �ֽ�ȭ �ð��� ���� �ð��� ���� �������� �ð� - ���� �������� �ǰڴ�.
	m_CurTime = (float)(pFbx->Ani_Vec[m_ClipInx]->Stime.GetSecondDouble() + m_UpdateTime);

	int iFrameInx = (int)(m_CurTime* m_FrameCnt);
	int iNextFrameInx = 0;

	// ���� �������� �������� ������ ũ�� 0���� �ʱ�ȭ
	if (iFrameInx >= pFbx->Ani_Vec[m_ClipInx]->Length_Time - 1)
	{
		iFrameInx = 0;
	}

	// �翬������ ���� ����� + 1 �������� �ǰڴ�.
	iNextFrameInx = iFrameInx + 1;

	for (size_t i = 0; i < m_pLoader->m_BoneCnt; i++)
	{
		if (pFbx->Bone_Vec[i]->KFVec.empty())
		{
			m_MXData_CurAni[i] = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Bone_MX);
			continue;
		}

		KeyFrame& CFrame = pFbx->Bone_Vec[i]->KFVec[iFrameInx];
		KeyFrame& NFrame = pFbx->Bone_Vec[i]->KFVec[iNextFrameInx];

		float fCFTime = (float)CFrame.dTime;
		float fNFTime = (float)NFrame.dTime;

		// ����ġ�� ���� ����
		float fPercent = (m_CurTime - fCFTime) / (1.0f / m_FrameCnt);

		// �׶��� ũ�� ��� ���� - ����
		DirectX::XMVECTOR vSC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetS());
		DirectX::XMVECTOR vSN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetS());

		// �׶��� ��ġ ��� ���� - ����
		DirectX::XMVECTOR vTC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetT());
		DirectX::XMVECTOR vTN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetT());

		// �׶��� ȸ�� ��� ���� - ����
		// �� ȸ���� ���ʹϾ����� �޾ƿ� 
		// �ϼ��� ���� ��Ŀ��� ȸ������� ���´ٴ� �� �Ұ��ɿ� �����
		// ���п���, �����(����, ����: quaternion ���ʹϾ�[*]) �Ǵ� �ع��� ��(����: Hamilton number)��
		// ���Ҽ��� Ȯ���� ���� �� ü���̴�.�� ���� �Ǽ� ������ ������,
		// ������ ������ ���չ�Ģ �� ������ ��ȯ��Ģ�� ������Ű���� ������ ��ȯ��Ģ�� �������� �ʴ´�.
		DirectX::XMVECTOR vQC = FBXLoader::FQTtoKVec(CFrame.MX_Frame.GetQ());
		DirectX::XMVECTOR vQN = FBXLoader::FQTtoKVec(NFrame.MX_Frame.GetQ());


		// �� �̺κ��� ���� ���ִ� ���� -> ���� ���� �׸��� �� ���� �־��ָ� �ڵ����� �������ش�.
		// ���� ���̷�Ʈ X�̴� �ȿ��� � ���� �Ͼ�� ������ -> �������� ����
		DirectX::XMVECTOR vS = DirectX::XMVectorLerp(vSC, vSN, fPercent);
		DirectX::XMVECTOR vT = DirectX::XMVectorLerp(vTC, vTN, fPercent);
		DirectX::XMVECTOR vQ = DirectX::XMQuaternionSlerp(vQC, vQN, fPercent);

		DirectX::XMVECTOR vZero = DirectX::XMVectorSet(.0f, .0f, .0f, .0f);

		// �⺻ �����¿� ������ ���� ����� ���� -���� ����
		// �� �� ���ϴ� ������ ũ���� ���θ� �����ִ� �༮�� ���̷�Ʈ�� �ִ�.
		KMatrix OffMat = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Offset_MX);
		m_MXData_CurAni[i] = OffMat * DirectX::XMMatrixAffineTransformation(vS, vZero, vQ, vT);
	}

	// ���� �� �ؽ��Ŀ� ������ ������� �ִ´�. -������ ũ���� ����� ���̴�.
	m_pBoneTex->Set_Pixel(&m_MXData_CurAni[0], sizeof(KMatrix) * m_MXData_CurAni.size());
}

void Renderer_BonAni::Render(KPtr<Camera> _Cam)
{
	if (nullptr != m_pBoneTex)
	{
		// ������ �ؽ��Ĵ� 10���� �Ҵ��Ѵ�.
		m_pBoneTex->Update(10);
	}
}