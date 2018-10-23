#include "KFBX.h"
#include <atlstr.h>
#include "KMacro.h"
#include "DXContainer_DE.h"


FBXLoader::FBXLoader()
{

}
FBXLoader::~FBXLoader()
{

}


void FBXLoader::Load(const wchar_t* _Path)
{
	m_pNewFbx = new KFBX();
	m_pManager = FbxManager::Create();

	// 불러올 정보로는
	// 버텍스(컬러, 노말, 바이노말, 탄젠트, 컬러 등) 인덱스 
	// 쉐이더
	// 재질
	// 1. 텍스쳐
	// 2. 댑스스텐실- 여기부터는 별도의 정보들 - FBX 에 없다 - 그러니 만들어야함
	// 3. 블랜드
	// 4. 레스터라이즈

	// 플렛폼 별 불러오는 세팅이 다르니 그걸 세팅해준다.
	FbxIOSettings* pIos = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIos);



	// 내부에서 간헐적으로 신과 임포터를 연다. 어캐보면 박스 2D와 같은 방식
	m_pScene = FbxScene::Create(m_pManager, "");
	FbxImporter* pImp = FbxImporter::Create(m_pManager, "");

	// m_psz -> 자동 string 변환 -> wstring에서 char* 형으로
	bool Temp = pImp->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings());
	bool Temp2 = pImp->Import(m_pScene);


	// 좌표축 계가 제품마다 또는 플렛폼별로 다를 수가 있기 떄문에 맞춰주는 작업
	// 단, 너무 맹신하진 말자.
	if (m_pScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::eMax)
	{
		m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eMax);
	}

	Count_AllBone(m_pScene->GetRootNode());

	// 본 개수 만큼 벡터 맞추기
	m_pNewFbx->Bone_Vec.reserve(m_BoneCnt);

	// 삼각화
	Triangulate(m_pScene->GetRootNode());

	// 본과 애니정보를 가져옴
	Load_Bone(m_pScene->GetRootNode(), 0, nullptr);
	Check_Animation();

	// 메쉬 데이터
	Set_MeshData(m_pScene->GetRootNode());


	pImp->Destroy();
	m_pScene->Destroy();
	pIos->Destroy();
	m_pManager->Destroy();

	delete m_pNewFbx;
}

// 제귀 돌려서 본의 전체 개수를 알아온다.
void FBXLoader::Count_AllBone(FbxNode* _pNode)
{
	++m_BoneCnt;
	int Cnt = _pNode->GetChildCount();
	for (int i = 0; i < Cnt; i++)
	{
		Count_AllBone(_pNode->GetChild(i));
	}
}
void FBXLoader::Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	KBone* NBone = nullptr;


	// 노드에 뼈가 있는지 없는지 검사함
	if (nullptr != pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		NBone = new KBone();
		NBone->Name = CA2W(_pNode->GetName(), CP_UTF8).m_psz;
		NBone->Depth = _Depth++; // 후위 증가인 이유는 -> Parent에서 1 늘려주고 들어오기 때문
		NBone->Index = (int)m_pNewFbx->Bone_Vec.size();
		NBone->m_pPBone = _pParent;
		m_pNewFbx->Bone_Vec.push_back(NBone);
		m_pNewFbx->Bone_Map.insert(std::multimap<std::wstring, KBone*>::value_type(NBone->Name, NBone));
	}
	else
	{
		_Depth++;
	}

	// Child Count - 제귀
	int CC = _pNode->GetChildCount();
	for (int i = 0; i < CC; i++)
	{
		Load_Bone(_pNode->GetChild(i), _Depth, NBone);
	}
}

void FBXLoader::Check_Animation()
{
	m_pScene->FillAnimStackNameArray(m_pNewFbx->AniName_Arr);

	for (int i = 0; i < m_pNewFbx->AniName_Arr.GetCount(); i++)
	{
		FbxAnimStack* TempStack = m_pScene->FindMember<FbxAnimStack>(m_pNewFbx->AniName_Arr[i]->Buffer());

		// 애니메이션이 없을 수도 있다 -> 건물 같은거
		if (nullptr == TempStack)
		{
			continue;
		}

		Animation_Info* NewInfo = new Animation_Info();
		NewInfo->Name = CA2W(TempStack->GetName());

		// 애니메이션의 시작과 끝의 시간을 받아옴
		FbxTakeInfo* TempInfo = m_pScene->GetTakeInfo(TempStack->GetName());
		NewInfo->Stime = TempInfo->mLocalTimeSpan.GetStart();
		NewInfo->Etime = TempInfo->mLocalTimeSpan.GetStop();

		NewInfo->eMode = m_pScene->GetGlobalSettings().GetTimeMode();
		NewInfo->Length_Time =
			NewInfo->Etime.GetFrameCount(NewInfo->eMode) - 
			NewInfo->Stime.GetFrameCount(NewInfo->eMode);
	}

}
void FBXLoader::Triangulate(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (nullptr == pAttr)
	{
		return; // BBY;
	}

	if (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		// 우리 쪽에서 쓰기 쉽게 변경해주는 작업
		// 일종의 컨버트 하는 과정이고 메쉬정보를 추출할 수 있다.
		FbxGeometryConverter NewCon(m_pManager);

		// 2번째 bool은 덮어 쓸거냐 말거냐를 묻는다.
		NewCon.Triangulate(pAttr, true);
	}

	int CC = _pNode->GetChildCount();
	for (int i = 0; i < CC; i++)
	{
		Triangulate(_pNode->GetChild(i));
	}
}


void FBXLoader::Set_MeshData(FbxNode* _pNode)
{
	FbxNodeAttribute* pAtttr = _pNode->GetNodeAttribute();


	// 해당 겟 함수는 메쉬를 들고오게 한다. ->
	// 단 리턴된는 해당 클래스는 오버로딩이 안 되어 있으므로 뒤에서 연산하는 것
	if (nullptr != pAtttr && pAtttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = _pNode->GetMesh();

		Mesh_FbxData* NewMD = new Mesh_FbxData();
		m_pNewFbx->MeshData_Vec.push_back(NewMD);

		NewMD->MName = CA2W(pMesh->GetName(), CP_UTF8);


		// 정점의 개수를 가져온다. - 단 겹치는 걸 세진 안흔ㄴ다.
		// 절대적인 버텍스 개수
		// control point -> 버텍스
		int iVtxCnt = pMesh->GetControlPointsCount();
		NewMD->Set_VertexCount(iVtxCnt);

		// 버텍스에서 Pos 포지션을 받아온다.
		FbxVector4* pFbxPos = pMesh->GetControlPoints();


		// 저장할 떄 char 배열을 한번에 저장 -> 통으로 불러오기 위함
		for (size_t i = 0; i < iVtxCnt; i++)
		{
			NewMD->PosVec[iVtxCnt].x = pFbxPos[i].mData[0];
			NewMD->PosVec[iVtxCnt].y = pFbxPos[i].mData[2];
			NewMD->PosVec[iVtxCnt].z = pFbxPos[i].mData[1];
			NewMD->PosVec[iVtxCnt].w = 1.0f;
		}


		// 인덱스 버퍼 - 삼각형 그리기
		int TriCount = pMesh->GetPolygonCount();


		// 서브 셋 개수 == 메터리얼 개수
		// 버텍스 * 3 = 인덱스 --> 물론 인덱스가 삼각형이 아닌 다른 방식(도형)으로 그려지면
		// 곱해지는 값이 달라진다. 그리고 결론적으로 저 곱해진 모든 값을 정상적으로 불러올 수 있어야함
		int MatCount = pMesh->GetNode()->GetMaterialCount();
		NewMD->IdxVec.resize(MatCount);

		// 정점정보가 속한 서브셋의 속성으 ㄹ알아내기 위한 정보
		// 메터리얼의 정보가 곧 서브셋의 속성을 이룬다?
		// 그 정점 좌표가 ㅇㅇ
		FbxGeometryElementMaterial* pMtrl = pMesh->GetElementMaterial();


		// 일단 삼각형이 아닌 도형을 그리지 않는다. -> 일단 기본적인 거라도 띄우는 식
		int IdxSize = pMesh->GetPolygonSize(0);
		if (3 != IdxSize)
		{
			BBY;
		}

		IDX32 tempIDX32 = {};
		KUINT CurVtx = 0;

		// 면의 개수
		for (size_t i = 0; i < TriCount; i++)
		{
			// 그 면의 인덱스 개수
			for (size_t j = 0; j < IdxSize; j++)
			{
				// 버텍스 순서대로 얻어와야 함
				tempIDX32.p[j] = pMesh->GetPolygonVertex(i, j);

				// 이 순서를 말함 -> 우리가 했던 그것들
				Set_Normal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
				Set_Tangent(pMesh, NewMD, tempIDX32.p[j], CurVtx);
				Set_BNormal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
				Set_Uv(pMesh, NewMD, tempIDX32.p[j], CurVtx);

				++CurVtx;
			} // for (size_t j = 0; j < IdxSize; j++)

			// 인덱스 버퍼를 얻어낸다. 위에서 버텍스를 얻었으니 그걸 
			// 인덱스로 그려야하는데 그 인덱스도 이렇게 받아오는 거다.
			KUINT SubIdx = pMtrl->GetIndexArray().GetAt(i);
			NewMD->IdxVec[SubIdx].push_back(tempIDX32.p[0]);
			NewMD->IdxVec[SubIdx].push_back(tempIDX32.p[2]);
			NewMD->IdxVec[SubIdx].push_back(tempIDX32.p[1]);
		} // for (size_t i = 0; i < TriCount; i++)


		// 위에서 버텍스 인덱스까지 그려낸 메쉬 하나에 대한 애니메이션 정보를
		// 가져왕야한다.
		if (false == m_pNewFbx->Ani_Map.empty())
		{
			Set_AniData(pMesh, NewMD);
		}
		// 
		else
		{
			NewMD->m_bAni = false;
		}

	} // if (nullptr != pAtttr && pAtttr->GetAttributeType() == FbxNodeAttribute::eMesh)
}



// Semantic
// MSDN 자료이고 HLSL에서 지원하는 형태 -> 디퍼드 쉐이더까지 했다면 굳이 이말이 뭔지 알거임.
// 디퍼드 혹은 포워드 쉐이더 같은 일종의 텍스쳐 합성을 하기 위해선 
// (여기서 합성이라 함은 Diffuse뿐만 아니라 Spec과 Bump효과를 말함)
// 텍스쳐가 여러 개 보내지는 게 핵심이다. 이건 이를 보낼 때의 형태이다.
// https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-semantics 

void FBXLoader::Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// 시멘틱에서 
	// Normal 0,1 ~ 될수 있음 여기서의 개수가 노말 개수
	int Count = _pMesh->GetElementNormalCount();

	// 그것조차 하나가 아니면 터뜨림 - 하나인 파일은 기본적인 파일임
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementNormal* pE = _pMesh->GetElementNormal();
	KUINT Idx = 0;

	// 여기서 또 짚고 넘어가야 할 점 - 버텍스 및 인덱스 저장방식도 
	// 다르게 저장될 수 있는점
	// 크게 두 가지의 옵션으로 나누는데 -> 아마 Max 옵션 별로니까 꼭 2개 뿐은 아닐 듯 엄청 많을 듯
	// 버텍스 순서로 저장되는 경우 혹은 인덱스 기준이 그것임

	// 버텍스 기준
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// 인덱스 기준
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// 인덱스
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// 아무래도 다 같은 코드를 반복해서 쓰기에 일단 이름을 맞추어 놓는다.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = vData.mData[1];

	// 우리는 여기서 w를 쓰지 않는다.
	_pMeshData->NormalVec[_CurIdx].w = .0f;
}


void FBXLoader::Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// 시멘틱에서 
	// Normal 0,1 ~ 될수 있음 여기서의 개수가 노말 개수
	int Count = _pMesh->GetElementTangentCount();

	// 그것조차 하나가 아니면 터뜨림 - 하나인 파일은 기본적인 파일임
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementTangent* pE = _pMesh->GetElementTangent();
	KUINT Idx = 0;

	// 여기서 또 짚고 넘어가야 할 점 - 버텍스 및 인덱스 저장방식도 
	// 다르게 저장될 수 있는점
	// 크게 두 가지의 옵션으로 나누는데 -> 아마 Max 옵션 별로니까 꼭 2개 뿐은 아닐 듯 엄청 많을 듯
	// 버텍스 순서로 저장되는 경우 혹은 인덱스 기준이 그것임

	// 버텍스 기준
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// 인덱스 기준
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// 인덱스
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// 아무래도 다 같은 코드를 반복해서 쓰기에 일단 이름을 맞추어 놓는다.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = vData.mData[1];

	// 우리는 여기서 w를 쓰지 않는다.
	_pMeshData->NormalVec[_CurIdx].w = .0f;
}


void FBXLoader::Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// 시멘틱에서 
	// Normal 0,1 ~ 될수 있음 여기서의 개수가 노말 개수
	int Count = _pMesh->GetElementBinormalCount();

	// 그것조차 하나가 아니면 터뜨림 - 하나인 파일은 기본적인 파일임
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementBinormal* pE = _pMesh->GetElementBinormal();
	KUINT Idx = 0;

	// 여기서 또 짚고 넘어가야 할 점 - 버텍스 및 인덱스 저장방식도 
	// 다르게 저장될 수 있는점
	// 크게 두 가지의 옵션으로 나누는데 -> 아마 Max 옵션 별로니까 꼭 2개 뿐은 아닐 듯 엄청 많을 듯
	// 버텍스 순서로 저장되는 경우 혹은 인덱스 기준이 그것임

	// 버텍스 기준
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// 인덱스 기준
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// 인덱스
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// 아무래도 다 같은 코드를 반복해서 쓰기에 일단 이름을 맞추어 놓는다.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = vData.mData[1];

	// 우리는 여기서 w를 쓰지 않는다.
	_pMeshData->NormalVec[_CurIdx].w = .0f;
}

void FBXLoader::Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// 시멘틱에서 
	// Normal 0,1 ~ 될수 있음 여기서의 개수가 노말 개수
	int Count = _pMesh->GetElementUVCount();

	// 그것조차 하나가 아니면 터뜨림 - 하나인 파일은 기본적인 파일임
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementUV* pE = _pMesh->GetElementUV();
	KUINT Idx = 0;

	// 여기서 또 짚고 넘어가야 할 점 - 버텍스 및 인덱스 저장방식도 
	// 다르게 저장될 수 있는점
	// 크게 두 가지의 옵션으로 나누는데 -> 아마 Max 옵션 별로니까 꼭 2개 뿐은 아닐 듯 엄청 많을 듯
	// 버텍스 순서로 저장되는 경우 혹은 인덱스 기준이 그것임

	// 버텍스 기준
	if (pE->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurVtx;
		}
		else
		{
			Idx = pE->GetIndexArray().GetAt(_CurVtx);
		}
	}
	// 인덱스 기준
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// 인덱스
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// 아무래도 다 같은 코드를 반복해서 쓰기에 일단 이름을 맞추어 놓는다.

	// UV 는 당여하지만 Vector2로 관리한다.
	FbxVector2 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];

	// 그리고 max 좌표계는 왼쪽 아래부터 잰다. 따라서 다음과 같은 보정이 필요함
	_pMeshData->NormalVec[_CurIdx].y = 1.0f - vData.mData[1];
}

void FBXLoader::Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData)
{
	// 스킨 -> 겹치는 부위
	// Deformer -> 변환기
	// BlendShape -> Zbrush 처럼 메쉬가 막 찌그러지는 형태
	// Skin -> 우리가 아는 그 애니메이션 - 접합부가있고 본이 있어 형태가 유지
	// VertexCacheDeformer -> 샘도 뭔지 모른다 즉 한번도 안 쓰는 경우임
	// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=cpp_ref/class_fbx_deformer.
	// html,topicNumber=cpp_ref_class_fbx_deformer_htmle6d8427a-4060-4b8a-9a26-4ece8baf83c4
	int Count = _pMesh->GetDeformerCount(FbxDeformer::eSkin);


	if (0 >= Count)
	{
		_pMeshData->m_bAni = false;
		return;
	}

	_pMeshData->m_bAni = true;

	for (int i = 0; i < Count; i++)
	{
		FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (nullptr == pSkin)
		{
			continue;
		}

		FbxSkin::EType Dtype = pSkin->GetSkinningType();

		// 물리적 애니 - 선형 애니
		// 물리적 애니란 오버워치 캐릭 죽었을 때처럼 흐느적거리는 거 말하는 거임
		// 그게 물리가 아니고 (물론 물리도 있겠지만) 본이 설치된 하나의 덩어리를 말하는 것
		if (FbxSkin::EType::eRigid == Dtype || Dtype == FbxSkin::EType::eLinear)
		{
			// 클러스터 -> 관절부위를 통칭 -> 결합체
			int iClusterCnt = pSkin->GetClusterCount();

			for (int j = 0; j < iClusterCnt; j++)
			{
				FbxCluster* pCluster = pSkin->GetCluster(j);

				if (nullptr == pCluster || nullptr == pCluster->GetLink())
				{
					continue;
				}

				// 그때의 본의 이름
				KBone* pBone = m_pNewFbx->Find_Bone(CA2W(pCluster->GetLink()->GetName()).m_psz);

				// 아니 스킨이 있는데 본이 없는 건 애초에 말이 안되잖아?
				// 는 말이 되는 경우가 있다.
				// 본이 없이 애니매이션이 가능한 경우가 있다 - 하지만 일반적인 경우는 아님
				// 일반적으로만 현재 고려하기에 터뜨리는 거임
				if (nullptr == pBone)
				{
					BBY;
				}

				FbxAMatrix NodeTrans = Get_FbxMatrix(_pMesh->GetNode());
			} // for (int j = 0; j < iClusterCnt; j++)
		} // if (FbxSkin::EType::eRigid == Dtype || Dtype == FbxSkin::EType::eLinear)
	} // for (int i = 0; i < Count; i++)
}



FbxAMatrix FBXLoader::Get_FbxMatrix(FbxNode* _pNode)
{
	const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);


	// 이게 자동으로 행렬 스케일링해서 보여주는 거임
	return FbxAMatrix(vT, vR, vS);
}