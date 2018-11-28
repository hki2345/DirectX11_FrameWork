#include "Renderer_BonAni.h"
#include "Texture.h"
#include "TimeManager.h"
#include "DXContainer_DE.h"
#include "ResourceManager.h"
#include "DebugManager.h"

#include "Core_Class.h"




Renderer_BonAni::Renderer_BonAni() : 
m_ClipInx(1), 
m_FrameCnt(30), 
m_UpdateSpd(.0f), 
m_UpdateTime(.0f),
m_MeshColor(KColor::White)
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_DATA;
	ROpt.IsBoneAni = 1;
}


Renderer_BonAni::~Renderer_BonAni()
{
}


void Renderer_BonAni::Set_Fbx(const wchar_t* _Name)
{
	MCon = ResourceManager<MeshContainer>::Find(_Name);
	Init_Mesh();
}


void Renderer_BonAni::Init_Mesh()
{
	for (size_t MeshInx = 0; MeshInx < MCon->m_Data.MeshVec.size(); MeshInx++)
	{
		KM3Mesh* pMD = MCon->m_Data.MeshVec[MeshInx];

		if (nullptr == pMD)
		{
			BBY;
		}

		KPtr<KMesh> NMesh = new KMesh();
		NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 버텍스 버퍼
		if (false == NMesh->Create_Vertex(
			pMD->VxCnt
			, pMD->VxSize
			, pMD->Usage
			, pMD->m_VertBD))
		{
			BBY;
		}

		for (size_t SubInx = 0; SubInx < pMD->IxVec.size(); SubInx++)
		{
			KM3Index* pInx = &pMD->IxVec[SubInx];

			NMesh->Create_Index(
				pInx->IxCnt
				, pInx->IxSize
				, pInx->Usage
				, pInx->Fm
				, pInx->m_InxBD
			);
		}


		// 해당 메쉬를 쌩으로 넣는 과정 ㅇㅇ
		Renderer::Set_Mesh(NMesh, (int)MeshInx);



		// 메터리얼 -> 메터리얼 개수만큼 또 씌워주어야 한다.
		for (KUINT MtlIdx = 0; MtlIdx < (KUINT)pMD->MtlVec.size(); MtlIdx++)
		{
			switch (ROpt.Defferd_orForward)
			{
			case 0:
				Set_Material(L"MESH3DMAT", MtlIdx);
				break;

			case 1:
				Set_Material(L"DEFFERD3DANIMAT", MtlIdx);
				break;

			default:
				break;
			}

			KPtr<KMaterial> CMat = material(MtlIdx);

			Material_FbxData* pData = &pMD->MtlVec[MtlIdx];

			std::wstring TMesh = PathManager::Find_ForderPath(L"Mesh");
			
			// 디퓨즈 받아오기
			if (pData->Diff[0] != 0)
			{
				std::wstring Tmp = pData->Diff;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, Tex->FileForder());
			}


			// 범프 받아오기
			if (pData->Bump[0] != 0)
			{
				std::wstring Tmp = pData->Bump;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, Tex->FileForder());
			}


			// 스펙 받아오기
			if (pData->Spec[0] != 0)
			{
				std::wstring Tmp = pData->Spec;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_SPEC, 2, Tex->FileForder());
			}

			// 이머 받아오기
			if (pData->Emiv[0] != 0)
			{
				std::wstring Tmp = pData->Emiv;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_EMIS, 3, Tex->FileForder());
			}

			// 오프셋 개수가 메터리얼 개수다. -> 오프셋은 기본 메쉬라 봐도 무방
			Insert_RenderData((KUINT)MeshInx, 0, MtlIdx, MtlIdx);
		}
	}

	if (0 >= MCon->m_Data.AniVec.size())
	{
		return;
	}

	m_pBoneTex = new Texture();
	m_pBoneTex->Create(
		(KUINT)MCon->m_Data.BoneVec.size() * 4, 1
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT
		, D3D11_USAGE_DYNAMIC
	);


	m_BoneData_CurAni.resize(MCon->m_Data.BoneVec.size());
	m_MXData_CurAni.resize(MCon->m_Data.BoneVec.size());

}

// 일단, 듸우는 거에 초점을 맞춘다.
void Renderer_BonAni::Load_FbxTest(const wchar_t* _Path)
{
	MCon = ResourceManager<MeshContainer>::Load(_Path);
	Init_Mesh();
}

void Renderer_BonAni::PrevUpdate()
{
	if (nullptr != m_CurAni)
	{
		PrevUpdate_Ani();
	}
}

void Renderer_BonAni::PrevUpdate_Ani()
{
	if (0 >= MCon->m_Data.BoneVec.size())
	{
		return;
	}

	// 시작 프레임
	m_CurTime =
		m_CurAni->Start / m_FrameCnt +
		m_UpdateTime +
		(float)(MCon->m_Data.AniVec[m_ClipInx].Stime.GetSecondDouble());
	m_UpdateTime += DELTATIME;

	if (m_UpdateTime >= MCon->m_Data.AniVec[m_ClipInx].Length_Time)
	{
		m_UpdateTime = .0f;
	}

	// 현재 시작 시간에 최신화 시간을 더한 시간이 현재 진행중인 시간 - 현재 프레임이 되겠다.
	m_CurTime = (float)(MCon->m_Data.AniVec[m_ClipInx].Stime.GetSecondDouble() + m_UpdateTime);
	

	int iFrameInx = (int)(m_CurTime * m_FrameCnt);
	int iNextFrameInx = 0;
	
	// 현재 프레임이 프레임의 끝보다 크면 0으로 초기화
	if (iFrameInx >= m_CurAni->End - 1)
	{
		m_UpdateTime = .0f; 		
		iFrameInx = m_CurAni->Start;
	}

	// 당연하지만 다음 장면은 + 1 프레임이 되겠다.
	iNextFrameInx = iFrameInx + 1;

	for (size_t i = 0; i < MCon->m_Data.BoneVec.size(); i++)
	{
		if (MCon->m_Data.BoneVec[i]->KFrameVec.empty())
		{
			m_MXData_CurAni[i] = MCon->m_Data.BoneVec[i]->BoneMX;
			continue;
		}

		KeyFrame& CFrame = MCon->m_Data.BoneVec[i]->KFrameVec[iFrameInx];
		KeyFrame& NFrame = MCon->m_Data.BoneVec[i]->KFrameVec[iNextFrameInx];

		float fCFTime = (float)CFrame.dTime;
		float fNFTime = (float)NFrame.dTime;

		// 가중치에 대한 비율
		float fPercent = (m_CurTime - fCFTime) / (1.0f / m_FrameCnt);


		// 해당 과정이 KM3 클래스로 빠졌기 때문에 주석 처리
		//// 그때의 크기 행렬 현재 - 다음
		//DirectX::XMVECTOR vSC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetS());
		//DirectX::XMVECTOR vSN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetS());

		//// 그때의 위치 행렬 현재 - 다음
		//DirectX::XMVECTOR vTC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetT());
		//DirectX::XMVECTOR vTN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetT());

		//// 그때의 회전 행렬 현재 - 다음
		//// 단 회전은 쿼터니언으로 받아옴 
		//// 완성된 월드 행렬에서 회전행렬을 빼온다는 건 불가능에 가까움
		//// 수학에서, 사원수(四元數, 영어: quaternion 쿼터니언[*]) 또는 해밀턴 수(영어: Hamilton number)는
		//// 복소수를 확장해 만든 수 체계이다.네 개의 실수 성분을 가지며,
		//// 덧셈과 곱셈의 결합법칙 및 덧셈의 교환법칙을 만족시키지만 곱셈의 교환법칙은 성립하지 않는다.
		//DirectX::XMVECTOR vQC = FBXLoader::FQTtoKVec(CFrame.MX_Frame.GetQ());
		//DirectX::XMVECTOR vQN = FBXLoader::FQTtoKVec(NFrame.MX_Frame.GetQ());


		//// 자 이부분이 보간 해주는 구간 -> 현재 다음 그리고 그 비율 넣어주면 자동으로 보간해준다.
		//// ㅁㅊ 다이렉트 X이다 안에서 어떤 일이 일어날까 ㅋㅋㅋ -> 선형보간 ㅇㅇ
		DirectX::XMVECTOR vS = DirectX::XMVectorLerp(CFrame.Scale, NFrame.Scale, fPercent);
		DirectX::XMVECTOR vT = DirectX::XMVectorLerp(CFrame.Pos, NFrame.Pos, fPercent);
		DirectX::XMVECTOR vQ = DirectX::XMQuaternionSlerp(CFrame.Rotate, NFrame.Rotate, fPercent);

		DirectX::XMVECTOR vZero = DirectX::XMVectorSet(.0f, .0f, .0f, 1.0f);

		// 기본 오프셋에 구해진 보간 행렬을 구함 -동작 구현
		// 단 그 곱하는 과정도 크자이 공부를 곱해주는 녀석이 다이렉트에 있다.
		// KMatrix OffMat = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Offset_FMX);
		m_BoneData_CurAni[i] = DirectX::XMMatrixAffineTransformation(vS, vZero, vQ, vT);
		m_MXData_CurAni[i] = MCon->m_Data.BoneVec[i]->OffsetMX * m_BoneData_CurAni[i];
	}

	m_pBoneTex->Set_Pixel(&m_MXData_CurAni[0], sizeof(KMatrix) * m_MXData_CurAni.size());


	// 세력 설정하는 부분
	m_MeshColor = KColor::Blue;
	Core_Class::MainDevice().SettingCB<KColor>(L"FORCE_COLOR", m_MeshColor, SHTYPE::ST_PS);
}


void Renderer_BonAni::RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	if (nullptr != m_pBoneTex)
	{
		// 데이터 텍스쳐는 10번에 할당한다.
		m_pBoneTex->Update(10);
	}
}



KMatrix Renderer_BonAni::Get_BoneMX(const wchar_t* _Name)
{
	KM3Bone* pBone = MCon->Find_Bone(_Name);

	return m_BoneData_CurAni[pBone->Index];
}


KMatrix Renderer_BonAni::Get_WBoneMX(const wchar_t* _Name)
{
	return Get_BoneMX(_Name) * Trans()->worldmat_const();
}



/********************************** 지정 애니메이션*********************************/
bool Renderer_BonAni::Set_AniChanger(const wchar_t* _Name)
{
	if (0 == _Name[0])
	{
		BBY;
		return false;
	}

	std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::iterator FI = m_ACMap.find(_Name);
	if (m_ACMap.end() == FI)
	{
		BBY;
		return false;
	}

	m_CurAni = FI->second;

	return true;
}

bool Renderer_BonAni::Erase_AniChanger(const wchar_t* _Name)
{
	if (0 == _Name[0])
	{
		BBY;
		return false;
	}

	std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::iterator FI = m_ACMap.find(_Name);
	if (m_ACMap.end() == FI)
	{
		BBY;
		return false;
	}


	m_ACMap.erase(FI);
	return true;
}

KPtr<Renderer_BonAni::Ani_Changer> Renderer_BonAni::Create_AniChanger(const wchar_t* _Name, const int& _Start, const int& _End)
{
	int TStart = _Start;
	int TEnd = _End;

	// 이거 -> 최대치 넘어가면 그냥 조절 - 터지는 거 막기
	if(TEnd >= MCon->m_Data.AniVec[m_ClipInx].Length_Time)
	{
		TEnd = (int)MCon->m_Data.AniVec[m_ClipInx].Length_Time;
	}
	if (TStart >= MCon->m_Data.AniVec[m_ClipInx].Length_Time - 1)
	{
		TEnd = (int)MCon->m_Data.AniVec[m_ClipInx].Length_Time - 1;
	}

	if (TEnd < TStart)
	{
		int Temp = TStart;
		TStart = TEnd;
		TEnd = Temp;
	}



	if (0 == _Name[0])
	{
		BBY;
		return nullptr;
	}

	Renderer_BonAni::Ani_Changer* NC = new Ani_Changer(_Name, TStart, TEnd);

	m_ACMap.insert(std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::value_type(NC->ws_name(), NC));

	return NC;
}

KPtr<Renderer_BonAni::Ani_Changer> Renderer_BonAni::Find_AniChamnger(const wchar_t* _Name)
{
	std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::iterator FI = m_ACMap.find(_Name);

	if (m_ACMap.end() == FI)
	{
		return nullptr;
	}

	return FI->second;
}


void Renderer_BonAni::Set_TexturePath(const TEX_TYPE& _Value, const wchar_t* _Path)
{
	for (size_t MeshInx = 0; MeshInx < MCon->m_Data.MeshVec.size(); MeshInx++)
	{
		KM3Mesh* pMD = MCon->m_Data.MeshVec[MeshInx];
		

		// 메터리얼 -> 메터리얼 개수만큼 또 씌워주어야 한다.
		for (KUINT MtlIdx = 0; MtlIdx < (KUINT)pMD->MtlVec.size(); MtlIdx++)
		{
			Material_FbxData* pData = &pMD->MtlVec[MtlIdx];

			switch (_Value)
			{
			case TEX_TYPE::TEX_COLOR:
				memcpy_s(pData->Diff, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			case TEX_TYPE::TEX_BUMP:
				memcpy_s(pData->Bump, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			case TEX_TYPE::TEX_SPEC:
				memcpy_s(pData->Spec, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			case TEX_TYPE::TEX_EMIS:
				memcpy_s(pData->Emiv, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			default:
				break;
			}
		}
	}
}