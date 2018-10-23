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

	// �ҷ��� �����δ�
	// ���ؽ�(�÷�, �븻, ���̳븻, ź��Ʈ, �÷� ��) �ε��� 
	// ���̴�
	// ����
	// 1. �ؽ���
	// 2. �𽺽��ٽ�- ������ʹ� ������ ������ - FBX �� ���� - �׷��� ��������
	// 3. ����
	// 4. �����Ͷ�����

	// �÷��� �� �ҷ����� ������ �ٸ��� �װ� �������ش�.
	FbxIOSettings* pIos = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIos);



	// ���ο��� ���������� �Ű� �����͸� ����. ��ĳ���� �ڽ� 2D�� ���� ���
	m_pScene = FbxScene::Create(m_pManager, "");
	FbxImporter* pImp = FbxImporter::Create(m_pManager, "");

	// m_psz -> �ڵ� string ��ȯ -> wstring���� char* ������
	bool Temp = pImp->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings());
	bool Temp2 = pImp->Import(m_pScene);


	// ��ǥ�� �谡 ��ǰ���� �Ǵ� �÷������� �ٸ� ���� �ֱ� ������ �����ִ� �۾�
	// ��, �ʹ� �ͽ����� ����.
	if (m_pScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::eMax)
	{
		m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eMax);
	}

	Count_AllBone(m_pScene->GetRootNode());

	// �� ���� ��ŭ ���� ���߱�
	m_pNewFbx->Bone_Vec.reserve(m_BoneCnt);

	// �ﰢȭ
	Triangulate(m_pScene->GetRootNode());

	// ���� �ִ������� ������
	Load_Bone(m_pScene->GetRootNode(), 0, nullptr);
	Check_Animation();

	// �޽� ������
	Set_MeshData(m_pScene->GetRootNode());


	pImp->Destroy();
	m_pScene->Destroy();
	pIos->Destroy();
	m_pManager->Destroy();

	delete m_pNewFbx;
}

// ���� ������ ���� ��ü ������ �˾ƿ´�.
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


	// ��忡 ���� �ִ��� ������ �˻���
	if (nullptr != pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		NBone = new KBone();
		NBone->Name = CA2W(_pNode->GetName(), CP_UTF8).m_psz;
		NBone->Depth = _Depth++; // ���� ������ ������ -> Parent���� 1 �÷��ְ� ������ ����
		NBone->Index = (int)m_pNewFbx->Bone_Vec.size();
		NBone->m_pPBone = _pParent;
		m_pNewFbx->Bone_Vec.push_back(NBone);
		m_pNewFbx->Bone_Map.insert(std::multimap<std::wstring, KBone*>::value_type(NBone->Name, NBone));
	}
	else
	{
		_Depth++;
	}

	// Child Count - ����
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

		// �ִϸ��̼��� ���� ���� �ִ� -> �ǹ� ������
		if (nullptr == TempStack)
		{
			continue;
		}

		Animation_Info* NewInfo = new Animation_Info();
		NewInfo->Name = CA2W(TempStack->GetName());

		// �ִϸ��̼��� ���۰� ���� �ð��� �޾ƿ�
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
		// �츮 �ʿ��� ���� ���� �������ִ� �۾�
		// ������ ����Ʈ �ϴ� �����̰� �޽������� ������ �� �ִ�.
		FbxGeometryConverter NewCon(m_pManager);

		// 2��° bool�� ���� ���ų� ���ųĸ� ���´�.
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


	// �ش� �� �Լ��� �޽��� ������ �Ѵ�. ->
	// �� ���ϵȴ� �ش� Ŭ������ �����ε��� �� �Ǿ� �����Ƿ� �ڿ��� �����ϴ� ��
	if (nullptr != pAtttr && pAtttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = _pNode->GetMesh();

		Mesh_FbxData* NewMD = new Mesh_FbxData();
		m_pNewFbx->MeshData_Vec.push_back(NewMD);

		NewMD->MName = CA2W(pMesh->GetName(), CP_UTF8);


		// ������ ������ �����´�. - �� ��ġ�� �� ���� ���礤��.
		// �������� ���ؽ� ����
		// control point -> ���ؽ�
		int iVtxCnt = pMesh->GetControlPointsCount();
		NewMD->Set_VertexCount(iVtxCnt);

		// ���ؽ����� Pos �������� �޾ƿ´�.
		FbxVector4* pFbxPos = pMesh->GetControlPoints();


		// ������ �� char �迭�� �ѹ��� ���� -> ������ �ҷ����� ����
		for (size_t i = 0; i < iVtxCnt; i++)
		{
			NewMD->PosVec[iVtxCnt].x = pFbxPos[i].mData[0];
			NewMD->PosVec[iVtxCnt].y = pFbxPos[i].mData[2];
			NewMD->PosVec[iVtxCnt].z = pFbxPos[i].mData[1];
			NewMD->PosVec[iVtxCnt].w = 1.0f;
		}


		// �ε��� ���� - �ﰢ�� �׸���
		int TriCount = pMesh->GetPolygonCount();


		// ���� �� ���� == ���͸��� ����
		// ���ؽ� * 3 = �ε��� --> ���� �ε����� �ﰢ���� �ƴ� �ٸ� ���(����)���� �׷�����
		// �������� ���� �޶�����. �׸��� ��������� �� ������ ��� ���� ���������� �ҷ��� �� �־����
		int MatCount = pMesh->GetNode()->GetMaterialCount();
		NewMD->IdxVec.resize(MatCount);

		// ���������� ���� ������� �Ӽ��� ���˾Ƴ��� ���� ����
		// ���͸����� ������ �� ������� �Ӽ��� �̷��?
		// �� ���� ��ǥ�� ����
		FbxGeometryElementMaterial* pMtrl = pMesh->GetElementMaterial();


		// �ϴ� �ﰢ���� �ƴ� ������ �׸��� �ʴ´�. -> �ϴ� �⺻���� �Ŷ� ���� ��
		int IdxSize = pMesh->GetPolygonSize(0);
		if (3 != IdxSize)
		{
			BBY;
		}

		IDX32 tempIDX32 = {};
		KUINT CurVtx = 0;

		// ���� ����
		for (size_t i = 0; i < TriCount; i++)
		{
			// �� ���� �ε��� ����
			for (size_t j = 0; j < IdxSize; j++)
			{
				// ���ؽ� ������� ���;� ��
				tempIDX32.p[j] = pMesh->GetPolygonVertex(i, j);

				// �� ������ ���� -> �츮�� �ߴ� �װ͵�
				Set_Normal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
				Set_Tangent(pMesh, NewMD, tempIDX32.p[j], CurVtx);
				Set_BNormal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
				Set_Uv(pMesh, NewMD, tempIDX32.p[j], CurVtx);

				++CurVtx;
			} // for (size_t j = 0; j < IdxSize; j++)

			// �ε��� ���۸� ����. ������ ���ؽ��� ������� �װ� 
			// �ε����� �׷����ϴµ� �� �ε����� �̷��� �޾ƿ��� �Ŵ�.
			KUINT SubIdx = pMtrl->GetIndexArray().GetAt(i);
			NewMD->IdxVec[SubIdx].push_back(tempIDX32.p[0]);
			NewMD->IdxVec[SubIdx].push_back(tempIDX32.p[2]);
			NewMD->IdxVec[SubIdx].push_back(tempIDX32.p[1]);
		} // for (size_t i = 0; i < TriCount; i++)


		// ������ ���ؽ� �ε������� �׷��� �޽� �ϳ��� ���� �ִϸ��̼� ������
		// �����վ��Ѵ�.
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
// MSDN �ڷ��̰� HLSL���� �����ϴ� ���� -> ���۵� ���̴����� �ߴٸ� ���� �̸��� ���� �˰���.
// ���۵� Ȥ�� ������ ���̴� ���� ������ �ؽ��� �ռ��� �ϱ� ���ؼ� 
// (���⼭ �ռ��̶� ���� Diffuse�Ӹ� �ƴ϶� Spec�� Bumpȿ���� ����)
// �ؽ��İ� ���� �� �������� �� �ٽ��̴�. �̰� �̸� ���� ���� �����̴�.
// https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-semantics 

void FBXLoader::Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementNormalCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementNormal* pE = _pMesh->GetElementNormal();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
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
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = vData.mData[1];

	// �츮�� ���⼭ w�� ���� �ʴ´�.
	_pMeshData->NormalVec[_CurIdx].w = .0f;
}


void FBXLoader::Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementTangentCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementTangent* pE = _pMesh->GetElementTangent();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
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
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = vData.mData[1];

	// �츮�� ���⼭ w�� ���� �ʴ´�.
	_pMeshData->NormalVec[_CurIdx].w = .0f;
}


void FBXLoader::Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementBinormalCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementBinormal* pE = _pMesh->GetElementBinormal();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
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
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.
	FbxVector4 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = vData.mData[1];

	// �츮�� ���⼭ w�� ���� �ʴ´�.
	_pMeshData->NormalVec[_CurIdx].w = .0f;
}

void FBXLoader::Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx)
{
	// �ø�ƽ���� 
	// Normal 0,1 ~ �ɼ� ���� ���⼭�� ������ �븻 ����
	int Count = _pMesh->GetElementUVCount();

	// �װ����� �ϳ��� �ƴϸ� �Ͷ߸� - �ϳ��� ������ �⺻���� ������
	if (1 != Count)
	{
		BBY;
	}

	FbxGeometryElementUV* pE = _pMesh->GetElementUV();
	KUINT Idx = 0;

	// ���⼭ �� ¤�� �Ѿ�� �� �� - ���ؽ� �� �ε��� �����ĵ� 
	// �ٸ��� ����� �� �ִ���
	// ũ�� �� ������ �ɼ����� �����µ� -> �Ƹ� Max �ɼ� ���δϱ� �� 2�� ���� �ƴ� �� ��û ���� ��
	// ���ؽ� ������ ����Ǵ� ��� Ȥ�� �ε��� ������ �װ���

	// ���ؽ� ����
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
	// �ε��� ����
	else if (pE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pE->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			Idx = _CurIdx;
		}
		else
		{
			// �ε���
			Idx = pE->GetIndexArray().GetAt(_CurIdx);
		}
	}

	// �ƹ����� �� ���� �ڵ带 �ݺ��ؼ� ���⿡ �ϴ� �̸��� ���߾� ���´�.

	// UV �� �翩������ Vector2�� �����Ѵ�.
	FbxVector2 vData = pE->GetDirectArray().GetAt(Idx);
	_pMeshData->NormalVec[_CurIdx].x = vData.mData[0];

	// �׸��� max ��ǥ��� ���� �Ʒ����� ���. ���� ������ ���� ������ �ʿ���
	_pMeshData->NormalVec[_CurIdx].y = 1.0f - vData.mData[1];
}

void FBXLoader::Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData)
{
	// ��Ų -> ��ġ�� ����
	// Deformer -> ��ȯ��
	// BlendShape -> Zbrush ó�� �޽��� �� ��׷����� ����
	// Skin -> �츮�� �ƴ� �� �ִϸ��̼� - ���պΰ��ְ� ���� �־� ���°� ����
	// VertexCacheDeformer -> ���� ���� �𸥴� �� �ѹ��� �� ���� �����
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

		// ������ �ִ� - ���� �ִ�
		// ������ �ִ϶� ������ġ ĳ�� �׾��� ��ó�� ������Ÿ��� �� ���ϴ� ����
		// �װ� ������ �ƴϰ� (���� ������ �ְ�����) ���� ��ġ�� �ϳ��� ����� ���ϴ� ��
		if (FbxSkin::EType::eRigid == Dtype || Dtype == FbxSkin::EType::eLinear)
		{
			// Ŭ������ -> ���������� ��Ī -> ����ü
			int iClusterCnt = pSkin->GetClusterCount();

			for (int j = 0; j < iClusterCnt; j++)
			{
				FbxCluster* pCluster = pSkin->GetCluster(j);

				if (nullptr == pCluster || nullptr == pCluster->GetLink())
				{
					continue;
				}

				// �׶��� ���� �̸�
				KBone* pBone = m_pNewFbx->Find_Bone(CA2W(pCluster->GetLink()->GetName()).m_psz);

				// �ƴ� ��Ų�� �ִµ� ���� ���� �� ���ʿ� ���� �ȵ��ݾ�?
				// �� ���� �Ǵ� ��찡 �ִ�.
				// ���� ���� �ִϸ��̼��� ������ ��찡 �ִ� - ������ �Ϲ����� ���� �ƴ�
				// �Ϲ������θ� ���� ����ϱ⿡ �Ͷ߸��� ����
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


	// �̰� �ڵ����� ��� �����ϸ��ؼ� �����ִ� ����
	return FbxAMatrix(vT, vR, vS);
}