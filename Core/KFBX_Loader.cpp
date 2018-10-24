#include "KFBX.h"
#include <atlstr.h>
#include "KMacro.h"
#include "DXContainer_DE.h"
#include <algorithm>


FBXLoader::FBXLoader()
{

}
FBXLoader::~FBXLoader()
{

}

FbxAMatrix FBXLoader::MX_Reflect;

void FBXLoader::Init_FBXLoader()
{
	MX_Reflect[0] = FbxVector4{ 1, 0, 0, 0 };
	MX_Reflect[1] = FbxVector4{ 0, 0, 1, 0 };
	MX_Reflect[2] = FbxVector4{ 0, 1, 0, 0 };
	MX_Reflect[3] = FbxVector4{ 0, 0, 0, 1 };
}


void FBXLoader::Load_FBX(const wchar_t* _Path)
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

		// 애니메이션에 인덱스를 넣어 관리하는데 - 처음에는 테이크 1이므로 
		// 우선 1만 들어갈 검.. 하지만 ㅏㄴ중에 추가될 수 있으므로 보류
		NewInfo->Index = i;
		m_pNewFbx->Ani_Vec.push_back(NewInfo);
		m_pNewFbx->Ani_Map.insert(std::map<std::wstring, Animation_Info*>::value_type(NewInfo->Name, NewInfo));
	}

}
void FBXLoader::Triangulate(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	// 루트부터 애트리뷰트를 찾기 때무넹 없을 수 있다.
	// 이것도 반복 돌리기 때무네 자식에서 다 불러옴
	if (nullptr != pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
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

		if (nullptr != pMesh)
		{
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
			size_t Tmepb = sizeof(*pFbxPos);


			// 저장할 떄 char 배열을 한번에 저장 -> 통으로 불러오기 위함
			for (size_t i = 0; i < iVtxCnt; i++)
			{
				NewMD->PosVec[i].x = (float)pFbxPos[i].mData[0];
				NewMD->PosVec[i].y = (float)pFbxPos[i].mData[2];
				NewMD->PosVec[i].z = (float)pFbxPos[i].mData[1];
				NewMD->PosVec[i].w = 1.0f;
			}


			// 인덱스 버퍼 - 삼각형 그리기
			int TriCount = pMesh->GetPolygonCount();


			// 서브 셋 개수 == 메터리얼 개수
			// 버텍스 * 3 = 인덱스 --> 물론 인덱스가 삼각형이 아닌 다른 방식(도형)으로 그려지면
			// 곱해지는 값이 달라진다. 그리고 결론적으로 저 곱해진 모든 값을 정상적으로 불러올 수 있어야함
			int MtrlCount = pMesh->GetNode()->GetMaterialCount();
			NewMD->IdxVec.resize(MtrlCount);

			// 정점정보가 속한 서브셋의 속성으 ㄹ알아내기 위한 정보
			// 메터리얼의 정보가 곧 서브셋의 속성을 이룬다?
			// 그 정점 좌표가 ㅇㅇ
			FbxGeometryElementMaterial* pMtrl = pMesh->GetElementMaterial();


			// 정점 좌표에 대한 메터리얼 정보를 불러온다.
			NewMD->m_MaterialVec.reserve(MtrlCount);
			for (int i = 0; i < MtrlCount; i++)
			{
				FbxSurfaceMaterial* pMtrSur = _pNode->GetMaterial(i);

				Material_FbxData* NewMat = new Material_FbxData();

				NewMat->Info.Diff = Get_MaterialColor(pMtrSur, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
				NewMat->Info.Ambi = Get_MaterialColor(pMtrSur, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbient);
				NewMat->Info.Emiv = Get_MaterialColor(pMtrSur, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissive);
				NewMat->Info.Spec = Get_MaterialColor(pMtrSur, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecular);


				NewMat->Diff = Get_MaterialTexName(pMtrSur, FbxSurfaceMaterial::sDiffuse);
				NewMat->Ambi = Get_MaterialTexName(pMtrSur, FbxSurfaceMaterial::sAmbient);
				NewMat->Emiv = Get_MaterialTexName(pMtrSur, FbxSurfaceMaterial::sEmissive);
				NewMat->Spec = Get_MaterialTexName(pMtrSur, FbxSurfaceMaterial::sSpecular);

				NewMD->m_MaterialVec.push_back(NewMat);
			}


			// 일단 삼각형이 아닌 도형을 그리지 않는다. -> 일단 기본적인 거라도 띄우는 식
			int IdxSize = pMesh->GetPolygonSize(0);
			if (3 != IdxSize)
			{
				BBY;
			}

			IDX32 tempIDX32 = {};
			KUINT CurVtx = 0;

			// 면의 개수
			for (int i = 0; i < TriCount; i++)
			{
				// 그 면의 인덱스 개수
				for (int j = 0; j < IdxSize; j++)
				{
					// 버텍스 순서대로 얻어와야 함
					tempIDX32.p[j] = (DWORD)pMesh->GetPolygonVertex(i, j);

					// 이 순서를 말함 -> 우리가 했던 그것들
					Set_Normal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
					Set_Tangent(pMesh, NewMD, tempIDX32.p[j], CurVtx);
					Set_BNormal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
					Set_Uv(pMesh, NewMD, tempIDX32.p[j], CurVtx);

					++CurVtx;
				} // for (size_t j = 0; j < IdxSize; j++)

				// 인덱스 버퍼를 얻어낸다. 위에서 버텍스를 얻었으니 그걸 
				// 인덱스로 그려야하는데 그 인덱스도 이렇게 받아오는 거다.
				int SubIdx = pMtrl->GetIndexArray().GetAt(i);
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
		} // if(nullptr != pMesh)
	} // if (nullptr != pAtttr && pAtttr->GetAttributeType() == FbxNodeAttribute::eMesh)


	int CC = _pNode->GetChildCount();
	for (int i = 0; i < CC; i++)
	{
		Set_MeshData(_pNode->GetChild(i));
	}
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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = (float)vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = (float)vData.mData[1];

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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = (float)vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = (float)vData.mData[1];

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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = (float)vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = (float)vData.mData[1];

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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];

	// 그리고 max 좌표계는 왼쪽 아래부터 잰다. 따라서 다음과 같은 보정이 필요함
	_pMeshData->NormalVec[_CurIdx].y = (float)(1.0f - vData.mData[1]);
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
				// 결합부위의 정점을 Cluster라고 하고
				// Cluster가 모여서 joint == 관절 이룬다고 봐야한다.
				// Cluster (joint == 관절)를 얻어온다.

				// 
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

				pBone->Bone_MX = Get_FbxMatrix(_pMesh->GetNode());

				Set_WeightIndices(pCluster, pBone, _pMeshData);
				Set_OffSetMatrix(pCluster, pBone, _pMeshData);
				Set_FrameMatrix(_pMesh->GetNode(), pCluster, pBone, _pMeshData);
			} // for (int j = 0; j < iClusterCnt; j++)
		} // if (FbxSkin::EType::eRigid == Dtype || Dtype == FbxSkin::EType::eLinear)
	} // for (int i = 0; i < Count; i++)

	Check_WI(_pMesh, _pMeshData);
}

// 재질 정보
void FBXLoader::Set_Material(FbxNode* _pNode)
{

}




FbxAMatrix FBXLoader::Get_FbxMatrix(FbxNode* _pNode)
{
	const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);


	// 이게 자동으로 행렬 스케일링해서 보여주는 거임
	return FbxAMatrix(vT, vR, vS);
}


// 가중치 설정 - 함수가 다 있음
void FBXLoader::Set_WeightIndices(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	int IndexCnt = _pC->GetControlPointIndicesCount();

	// 그 가중치를 우리가 계산하는 게 아니고 클러스터를 직접 계산해 제공해준다.
	WI tempWi;
	for (int i = 0; i < IndexCnt; i++)
	{
		tempWi.BoneIdx = _pBone->Index;
		tempWi.dWeight = _pC->GetControlPointWeights()[i];
		tempWi.IndiIdx = _pC->GetControlPointIndices()[i];
		_pMeshData->WIVec[tempWi.IndiIdx].push_back(tempWi);
	}
}



// Offset - 애니메이션 모델의 기본 자세 상태
void FBXLoader::Set_OffSetMatrix(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	FbxAMatrix MX_ClusterTrans;
	FbxAMatrix MX_ClusterLinkTrans;

	// 관절과 그 관절을 잇는 트랜스
	_pC->GetTransformMatrix(MX_ClusterTrans);
	_pC->GetTransformLinkMatrix(MX_ClusterLinkTrans);

	FbxAMatrix MX_Offset;


	// 역행렬은? (1/2) * (2/1)
	// 각 클러스터단위의 기본적인 오프셋 행렬을 구하는 과정이 된다.
	// 노드와 링크된 다른 행렬의 역행렬 * 나의 행렬 * 본의 기본행렬
	// 즉, 변화 되어잇는 형태에서 이전의 역행렬 나의 행렬로 부위 초기화 후 기본행렬을 곱해 보정
	MX_Offset = MX_ClusterLinkTrans.Inverse() * MX_ClusterTrans * _pBone->Bone_MX;

	// 이건 행렬 좌표역시 y, z 가 바뀌어 있기 때문에 바꿔주는 것 ->
	// 두번 곱하는 이유는 먼저 행을 바꾸고 열을 바구기 때문에
	MX_Offset = MX_Reflect * MX_Offset * MX_Reflect;
	_pBone->Offset_MX = MX_Offset;
}


//  시간 별 애니메이션 설정
void FBXLoader::Set_FrameMatrix(FbxNode* _pNode, FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	// 타임모드를 가져와 - 그 타임의 모든 애니메이션을 싹다 가져온다.
	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	for (size_t i = 0; i < m_pNewFbx->Ani_Vec.size(); i++)
	{
		FbxLongLong SFrame = m_pNewFbx->Ani_Vec[i]->Stime.GetFrameCount(eTimeMode);
		FbxLongLong EFrame = m_pNewFbx->Ani_Vec[i]->Etime.GetFrameCount(eTimeMode);

		for (FbxLongLong i = SFrame; i < EFrame; i++)
		{
			KeyFrame tFrame = {};
			FbxTime tTime = 0;

			// SetFrame -> 타임 모드와 시간을 넣어주면 그에 해당하는
			// FBX의 애니메이션에서 사용할 수있는 시간대로 변환해줌
			tTime.SetFrame(i, eTimeMode);

			// 해당 클ㄹ허스터와 링크된 클러스터를 모두 시간단위로 변환해줌
			FbxAMatrix MX_FromTrans = _pNode->EvaluateGlobalTransform(tTime) * _pBone->Bone_MX;
			FbxAMatrix MX_CurTrans = MX_FromTrans.Inverse() * _pC->GetLink()->EvaluateGlobalTransform(tTime);


			// 이 프레임ㅇ 워크로 공간 변환
			MX_CurTrans = MX_Reflect * MX_CurTrans * MX_Reflect;

			tFrame.KeyTime = tTime.GetSecondDouble();
			tFrame.MX_Frame = MX_CurTrans;
			m_pNewFbx->Bone_Vec[_pBone->Index]->KFVec.push_back(tFrame);
		}
	}
}



void FBXLoader::Check_WI(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData)
{
	// 가중치 누적 계산 -> 가중치에 대한 판별은 우리가 직접한다.
	std::vector<std::vector<WI>>::iterator Iter = _pMeshData->WIVec.begin();

	for (size_t i = 0, iVtxInx = 0; i < _pMeshData->WIVec.size(); i++)
	{
		if (_pMeshData->WIVec[i].size() > 1)
		{
			// 벡터 정렬 -> 이거 쓰려면 Algoritm 헤더가 필요하다.
			// 그리고 이거 무명함수 특히 sort의 두번째 인자값이 바로 조건이 들어가는 부분인데
			// 그걸 무명함수로 해버림
			std::sort((*Iter).begin(), (*Iter).end(), 
				[](const WI& _left, const WI& _Right)
			{
				return _left.dWeight > _Right.dWeight;
			}
			);

			double dWeight = .0f;
			for (int i = 0; i < (int)(*Iter).size(); i++)
			{
				dWeight += (*Iter)[i].dWeight;
			}

			// 가중치의 합이 1보다 클수 없지만 - 0과 1사이의 값
			// 가중치 자체는 의미를 가지기 때문에 처음부분에 더해준다.
			// 만에하나 넘어가는 일이 있다면 다음 클러스터에 더해줌
			double revision = .0f;
			if (dWeight > 1.0)
			{
				revision = 1.0 - dWeight;
				(*Iter)[0].dWeight += revision;
			}

			if ((*Iter).size() >= 4)
			{
				(*Iter).erase((*Iter).begin() + 4, (*Iter).end());
			}
		} // if (_pMeshData->WIVec[i].size() > 1)

		KVector Weights = {};
		KVector Indices = {};

		for (int i = 0; i < (*Iter).size(); i++)
		{
			Weights.s[i] = (float)(*Iter)[i].dWeight;
			Indices.s[i] = (float)(*Iter)[i].BoneIdx;
		}

		_pMeshData->WeightsVec[iVtxInx] = Weights;
		_pMeshData->IndicesVec[iVtxInx] = Indices;
	} // for (size_t i = 0, iVtxInx = 0; i < _pMeshData->WIVec.size(); i++)
}



KVector FBXLoader::Get_MaterialColor(FbxSurfaceMaterial* _pFbxMatData,
	const char* _pMtrlColorName,
	const char* _pMtrlFactorName)
{
	FbxDouble3 FbxColor;
	FbxDouble dFactor = 0;

	FbxProperty tMtrlColorPro = _pFbxMatData->FindProperty(_pMtrlColorName);
	FbxProperty tMtrlFactorPro = _pFbxMatData->FindProperty(_pMtrlFactorName);

	if (tMtrlColorPro.IsValid())
	{
		FbxColor = tMtrlColorPro.Get<FbxDouble3>();
	}

	if (tMtrlFactorPro.IsValid())
	{
		dFactor = tMtrlFactorPro.Get<FbxDouble>();
	}

	// 색깔은 또 rgb로 정상적으로 가져온다.
	return KVector(
		(float)(FbxColor.mData[0] * dFactor),
		(float)(FbxColor.mData[1] * dFactor),
		(float)(FbxColor.mData[2] * dFactor),
		(float)dFactor
	);
}

std::wstring FBXLoader::Get_MaterialTexName(FbxSurfaceMaterial* _pFbxMatData,
	const char* _pMtrlTexName)
{
	// 텍스쳐 이름은 그냥 string인데
	// 문제는 텍스쳐 이름이 길다는 점이다. - 경로를 가져움
	std::string Name = "";

	FbxProperty tMtrlTexNamePro = _pFbxMatData->FindProperty(_pMtrlTexName);

	if (tMtrlTexNamePro.IsValid())
	{
		KUINT iCnt = tMtrlTexNamePro.GetSrcObjectCount();

		if (0 != iCnt)
		{
			FbxFileTexture* pFbxTex = tMtrlTexNamePro.GetSrcObject<FbxFileTexture>(0);
			if (nullptr != pFbxTex)
			{
				Name = pFbxTex->GetFileName();
			}
		}
	}

	return CA2W(Name.c_str()).m_psz;
}