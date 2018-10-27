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



// 일단, 듸우는 거에 초점을 맞춘다.
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

		// 버텍스 버퍼
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


		// 해당 메쉬를 쌩으로 넣는 과정 ㅇㅇ
		Set_Mesh(NMesh, (int)MeshInx);



		// 메터리얼 -> 메터리얼 개수만큼 또 씌워주어야 한다.
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

			// 디퓨즈 받아오기
			if (pData->Diff != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Diff.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, Tex->FileForder());
			}


			// 범프 받아오기
			if (pData->Bump != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Bump.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, Tex->FileForder());
			}


			// 스펙 받아오기
			if (pData->Spec != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Spec.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_SPEC, 2, Tex->FileForder());
			}

			// 오프셋 개수가 메터리얼 개수다. -> 오프셋은 기본 메쉬라 봐도 무방
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

	// 현재 시작 시간에 최신화 시간을 더한 시간이 현재 진행중인 시간 - 현재 프레임이 되겠다.
	m_CurTime = (float)(pFbx->Ani_Vec[m_ClipInx]->Stime.GetSecondDouble() + m_UpdateTime);

	int iFrameInx = (int)(m_CurTime* m_FrameCnt);
	int iNextFrameInx = 0;

	// 현재 프레임이 프레임의 끝보다 크면 0으로 초기화
	if (iFrameInx >= pFbx->Ani_Vec[m_ClipInx]->Length_Time - 1)
	{
		iFrameInx = 0;
	}

	// 당연하지만 다음 장면은 + 1 프레임이 되겠다.
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

		// 가중치에 대한 비율
		float fPercent = (m_CurTime - fCFTime) / (1.0f / m_FrameCnt);

		// 그때의 크기 행렬 현재 - 다음
		DirectX::XMVECTOR vSC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetS());
		DirectX::XMVECTOR vSN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetS());

		// 그때의 위치 행렬 현재 - 다음
		DirectX::XMVECTOR vTC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetT());
		DirectX::XMVECTOR vTN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetT());

		// 그때의 회전 행렬 현재 - 다음
		// 단 회전은 쿼터니언으로 받아옴 
		// 완성된 월드 행렬에서 회전행렬을 빼온다는 건 불가능에 가까움
		// 수학에서, 사원수(四元數, 영어: quaternion 쿼터니언[*]) 또는 해밀턴 수(영어: Hamilton number)는
		// 복소수를 확장해 만든 수 체계이다.네 개의 실수 성분을 가지며,
		// 덧셈과 곱셈의 결합법칙 및 덧셈의 교환법칙을 만족시키지만 곱셈의 교환법칙은 성립하지 않는다.
		DirectX::XMVECTOR vQC = FBXLoader::FQTtoKVec(CFrame.MX_Frame.GetQ());
		DirectX::XMVECTOR vQN = FBXLoader::FQTtoKVec(NFrame.MX_Frame.GetQ());


		// 자 이부분이 보간 해주는 구간 -> 현재 다음 그리고 그 비율 넣어주면 자동으로 보간해준다.
		// ㅁㅊ 다이렉트 X이다 안에서 어떤 일이 일어날까 ㅋㅋㅋ -> 선형보간 ㅇㅇ
		DirectX::XMVECTOR vS = DirectX::XMVectorLerp(vSC, vSN, fPercent);
		DirectX::XMVECTOR vT = DirectX::XMVectorLerp(vTC, vTN, fPercent);
		DirectX::XMVECTOR vQ = DirectX::XMQuaternionSlerp(vQC, vQN, fPercent);

		DirectX::XMVECTOR vZero = DirectX::XMVectorSet(.0f, .0f, .0f, .0f);

		// 기본 오프셋에 구해진 보간 행렬을 구함 -동작 구현
		// 단 그 곱하는 과정도 크자이 공부를 곱해주는 녀석이 다이렉트에 있다.
		KMatrix OffMat = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Offset_MX);
		m_MXData_CurAni[i] = OffMat * DirectX::XMMatrixAffineTransformation(vS, vZero, vQ, vT);
	}

	// 이제 그 텍스쳐에 데이터 방식으로 넣는다. -구해진 크자이 행렬을 말이다.
	m_pBoneTex->Set_Pixel(&m_MXData_CurAni[0], sizeof(KMatrix) * m_MXData_CurAni.size());
}

void Renderer_BonAni::Render(KPtr<Camera> _Cam)
{
	if (nullptr != m_pBoneTex)
	{
		// 데이터 텍스쳐는 10번에 할당한다.
		m_pBoneTex->Update(10);
	}
}