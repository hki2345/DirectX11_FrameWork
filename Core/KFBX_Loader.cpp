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

		// �ִϸ��̼ǿ� �ε����� �־� �����ϴµ� - ó������ ����ũ 1�̹Ƿ� 
		// �켱 1�� �� ��.. ������ �����߿� �߰��� �� �����Ƿ� ����
		NewInfo->Index = i;
		m_pNewFbx->Ani_Vec.push_back(NewInfo);
		m_pNewFbx->Ani_Map.insert(std::map<std::wstring, Animation_Info*>::value_type(NewInfo->Name, NewInfo));
	}

}
void FBXLoader::Triangulate(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	// ��Ʈ���� ��Ʈ����Ʈ�� ã�� ������ ���� �� �ִ�.
	// �̰͵� �ݺ� ������ ������ �ڽĿ��� �� �ҷ���
	if (nullptr != pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
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

		if (nullptr != pMesh)
		{
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
			size_t Tmepb = sizeof(*pFbxPos);


			// ������ �� char �迭�� �ѹ��� ���� -> ������ �ҷ����� ����
			for (size_t i = 0; i < iVtxCnt; i++)
			{
				NewMD->PosVec[i].x = (float)pFbxPos[i].mData[0];
				NewMD->PosVec[i].y = (float)pFbxPos[i].mData[2];
				NewMD->PosVec[i].z = (float)pFbxPos[i].mData[1];
				NewMD->PosVec[i].w = 1.0f;
			}


			// �ε��� ���� - �ﰢ�� �׸���
			int TriCount = pMesh->GetPolygonCount();


			// ���� �� ���� == ���͸��� ����
			// ���ؽ� * 3 = �ε��� --> ���� �ε����� �ﰢ���� �ƴ� �ٸ� ���(����)���� �׷�����
			// �������� ���� �޶�����. �׸��� ��������� �� ������ ��� ���� ���������� �ҷ��� �� �־����
			int MtrlCount = pMesh->GetNode()->GetMaterialCount();
			NewMD->IdxVec.resize(MtrlCount);

			// ���������� ���� ������� �Ӽ��� ���˾Ƴ��� ���� ����
			// ���͸����� ������ �� ������� �Ӽ��� �̷��?
			// �� ���� ��ǥ�� ����
			FbxGeometryElementMaterial* pMtrl = pMesh->GetElementMaterial();


			// ���� ��ǥ�� ���� ���͸��� ������ �ҷ��´�.
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


			// �ϴ� �ﰢ���� �ƴ� ������ �׸��� �ʴ´�. -> �ϴ� �⺻���� �Ŷ� ���� ��
			int IdxSize = pMesh->GetPolygonSize(0);
			if (3 != IdxSize)
			{
				BBY;
			}

			IDX32 tempIDX32 = {};
			KUINT CurVtx = 0;

			// ���� ����
			for (int i = 0; i < TriCount; i++)
			{
				// �� ���� �ε��� ����
				for (int j = 0; j < IdxSize; j++)
				{
					// ���ؽ� ������� ���;� ��
					tempIDX32.p[j] = (DWORD)pMesh->GetPolygonVertex(i, j);

					// �� ������ ���� -> �츮�� �ߴ� �װ͵�
					Set_Normal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
					Set_Tangent(pMesh, NewMD, tempIDX32.p[j], CurVtx);
					Set_BNormal(pMesh, NewMD, tempIDX32.p[j], CurVtx);
					Set_Uv(pMesh, NewMD, tempIDX32.p[j], CurVtx);

					++CurVtx;
				} // for (size_t j = 0; j < IdxSize; j++)

				// �ε��� ���۸� ����. ������ ���ؽ��� ������� �װ� 
				// �ε����� �׷����ϴµ� �� �ε����� �̷��� �޾ƿ��� �Ŵ�.
				int SubIdx = pMtrl->GetIndexArray().GetAt(i);
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
		} // if(nullptr != pMesh)
	} // if (nullptr != pAtttr && pAtttr->GetAttributeType() == FbxNodeAttribute::eMesh)


	int CC = _pNode->GetChildCount();
	for (int i = 0; i < CC; i++)
	{
		Set_MeshData(_pNode->GetChild(i));
	}
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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = (float)vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = (float)vData.mData[1];

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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = (float)vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = (float)vData.mData[1];

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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];
	_pMeshData->NormalVec[_CurIdx].y = (float)vData.mData[2];
	_pMeshData->NormalVec[_CurIdx].z = (float)vData.mData[1];

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
	_pMeshData->NormalVec[_CurIdx].x = (float)vData.mData[0];

	// �׸��� max ��ǥ��� ���� �Ʒ����� ���. ���� ������ ���� ������ �ʿ���
	_pMeshData->NormalVec[_CurIdx].y = (float)(1.0f - vData.mData[1]);
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
				// ���պ����� ������ Cluster��� �ϰ�
				// Cluster�� �𿩼� joint == ���� �̷�ٰ� �����Ѵ�.
				// Cluster (joint == ����)�� ���´�.

				// 
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

				pBone->Bone_MX = Get_FbxMatrix(_pMesh->GetNode());

				Set_WeightIndices(pCluster, pBone, _pMeshData);
				Set_OffSetMatrix(pCluster, pBone, _pMeshData);
				Set_FrameMatrix(_pMesh->GetNode(), pCluster, pBone, _pMeshData);
			} // for (int j = 0; j < iClusterCnt; j++)
		} // if (FbxSkin::EType::eRigid == Dtype || Dtype == FbxSkin::EType::eLinear)
	} // for (int i = 0; i < Count; i++)

	Check_WI(_pMesh, _pMeshData);
}

// ���� ����
void FBXLoader::Set_Material(FbxNode* _pNode)
{

}




FbxAMatrix FBXLoader::Get_FbxMatrix(FbxNode* _pNode)
{
	const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);


	// �̰� �ڵ����� ��� �����ϸ��ؼ� �����ִ� ����
	return FbxAMatrix(vT, vR, vS);
}


// ����ġ ���� - �Լ��� �� ����
void FBXLoader::Set_WeightIndices(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	int IndexCnt = _pC->GetControlPointIndicesCount();

	// �� ����ġ�� �츮�� ����ϴ� �� �ƴϰ� Ŭ�����͸� ���� ����� �������ش�.
	WI tempWi;
	for (int i = 0; i < IndexCnt; i++)
	{
		tempWi.BoneIdx = _pBone->Index;
		tempWi.dWeight = _pC->GetControlPointWeights()[i];
		tempWi.IndiIdx = _pC->GetControlPointIndices()[i];
		_pMeshData->WIVec[tempWi.IndiIdx].push_back(tempWi);
	}
}



// Offset - �ִϸ��̼� ���� �⺻ �ڼ� ����
void FBXLoader::Set_OffSetMatrix(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	FbxAMatrix MX_ClusterTrans;
	FbxAMatrix MX_ClusterLinkTrans;

	// ������ �� ������ �մ� Ʈ����
	_pC->GetTransformMatrix(MX_ClusterTrans);
	_pC->GetTransformLinkMatrix(MX_ClusterLinkTrans);

	FbxAMatrix MX_Offset;


	// �������? (1/2) * (2/1)
	// �� Ŭ�����ʹ����� �⺻���� ������ ����� ���ϴ� ������ �ȴ�.
	// ���� ��ũ�� �ٸ� ����� ����� * ���� ��� * ���� �⺻���
	// ��, ��ȭ �Ǿ��մ� ���¿��� ������ ����� ���� ��ķ� ���� �ʱ�ȭ �� �⺻����� ���� ����
	MX_Offset = MX_ClusterLinkTrans.Inverse() * MX_ClusterTrans * _pBone->Bone_MX;

	// �̰� ��� ��ǥ���� y, z �� �ٲ�� �ֱ� ������ �ٲ��ִ� �� ->
	// �ι� ���ϴ� ������ ���� ���� �ٲٰ� ���� �ٱ��� ������
	MX_Offset = MX_Reflect * MX_Offset * MX_Reflect;
	_pBone->Offset_MX = MX_Offset;
}


//  �ð� �� �ִϸ��̼� ����
void FBXLoader::Set_FrameMatrix(FbxNode* _pNode, FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData)
{
	// Ÿ�Ӹ�带 ������ - �� Ÿ���� ��� �ִϸ��̼��� �ϴ� �����´�.
	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	for (size_t i = 0; i < m_pNewFbx->Ani_Vec.size(); i++)
	{
		FbxLongLong SFrame = m_pNewFbx->Ani_Vec[i]->Stime.GetFrameCount(eTimeMode);
		FbxLongLong EFrame = m_pNewFbx->Ani_Vec[i]->Etime.GetFrameCount(eTimeMode);

		for (FbxLongLong i = SFrame; i < EFrame; i++)
		{
			KeyFrame tFrame = {};
			FbxTime tTime = 0;

			// SetFrame -> Ÿ�� ���� �ð��� �־��ָ� �׿� �ش��ϴ�
			// FBX�� �ִϸ��̼ǿ��� ����� ���ִ� �ð���� ��ȯ����
			tTime.SetFrame(i, eTimeMode);

			// �ش� Ŭ���㽺�Ϳ� ��ũ�� Ŭ�����͸� ��� �ð������� ��ȯ����
			FbxAMatrix MX_FromTrans = _pNode->EvaluateGlobalTransform(tTime) * _pBone->Bone_MX;
			FbxAMatrix MX_CurTrans = MX_FromTrans.Inverse() * _pC->GetLink()->EvaluateGlobalTransform(tTime);


			// �� �����Ӥ� ��ũ�� ���� ��ȯ
			MX_CurTrans = MX_Reflect * MX_CurTrans * MX_Reflect;

			tFrame.KeyTime = tTime.GetSecondDouble();
			tFrame.MX_Frame = MX_CurTrans;
			m_pNewFbx->Bone_Vec[_pBone->Index]->KFVec.push_back(tFrame);
		}
	}
}



void FBXLoader::Check_WI(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData)
{
	// ����ġ ���� ��� -> ����ġ�� ���� �Ǻ��� �츮�� �����Ѵ�.
	std::vector<std::vector<WI>>::iterator Iter = _pMeshData->WIVec.begin();

	for (size_t i = 0, iVtxInx = 0; i < _pMeshData->WIVec.size(); i++)
	{
		if (_pMeshData->WIVec[i].size() > 1)
		{
			// ���� ���� -> �̰� ������ Algoritm ����� �ʿ��ϴ�.
			// �׸��� �̰� �����Լ� Ư�� sort�� �ι�° ���ڰ��� �ٷ� ������ ���� �κ��ε�
			// �װ� �����Լ��� �ع���
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

			// ����ġ�� ���� 1���� Ŭ�� ������ - 0�� 1������ ��
			// ����ġ ��ü�� �ǹ̸� ������ ������ ó���κп� �����ش�.
			// �����ϳ� �Ѿ�� ���� �ִٸ� ���� Ŭ�����Ϳ� ������
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

	// ������ �� rgb�� ���������� �����´�.
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
	// �ؽ��� �̸��� �׳� string�ε�
	// ������ �ؽ��� �̸��� ��ٴ� ���̴�. - ��θ� ������
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