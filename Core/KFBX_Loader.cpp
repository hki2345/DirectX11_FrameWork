#include "KFBX.h"
#include <atlstr.h>
#include "KMacro.h"
#include "DXContainer.h"


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
	// 3. ������
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


	// �ش� �� �Լ��� �޽��� ������� �Ѵ�. ->
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
	}
}