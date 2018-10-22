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

	Load_Bone(m_pScene->GetRootNode(), 0, nullptr);
	Check_Animation();
	Triangulate(m_pScene->GetRootNode());

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
}

void FBXLoader::Check_Animation()
{

}
void FBXLoader::Triangulate(FbxNode* _pNode)
{

}
void FBXLoader::Set_MeshData(FbxMesh* _pMesh)
{

}



KFBX::KFBX()
{
	
}


KFBX::~KFBX()
{
}



void KFBX::Load(const wchar_t* _Path)
{
	// �������� ������ ��𼭵� �� �� �ְ� �Ԥ���
	m_pManager = FbxManager::Create();;

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
	FbxScene* pScene = FbxScene::Create(m_pManager, "");
	FbxImporter* pImp = FbxImporter::Create(m_pManager, "");

	// m_psz -> �ڵ� string ��ȯ -> wstring���� char* ������
	bool Temp = pImp->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings());
	bool Temp2 = pImp->Import(pScene);


	Load_Triangle(pScene->GetRootNode());

	pImp->Destroy();
	pScene->Destroy();
	pIos->Destroy();
	m_pManager->Destroy();
}


// FBX�� �ص� Ʈ�������� �Ǿ��ִ�.
void KFBX::Load_Triangle(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (nullptr == pAttr)
	{
		// BBY;
	}

	if (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		// �츮 �ʿ��� ���� ���� �������ִ� �۾�
		// ������ ����Ʈ �ϴ� �����̰� �޽������� ������ �� �ִ�.
		FbxGeometryConverter NewCon(m_pManager);

		// 2��° bool�� ���� ���ų� ���ųĸ� ���´�.
		NewCon.Triangulate(pAttr, true);

		FbxMesh* NewMesh = _pNode->GetMesh();
		Load_Mesh(NewMesh);
	}



	int MatCnt = _pNode->GetMaterialCount();

	if (MatCnt > 0)
	{
		// ���͸��� ���� �ε�
	}


	// Ʈ������ -> �ڽ��� ������ �װ� ���͵����� ������
	for (int i = 0; i < _pNode->GetChildCount(); i++)
	{
		Load_Triangle(_pNode->GetChild(i));
	}
}


void KFBX::Load_Mesh(FbxMesh* _pMesh)
{
	int iVtxCnt = _pMesh->GetControlPointsCount();

	FbxVector4* pVtxPos = _pMesh->GetControlPoints();

	KVector4 NewPos;


	// FBX���� �����ϴ� Vector�� x, z, y ���̴�.
	for (int i = 0; i < iVtxCnt; i++)
	{
		NewPos.x = (float)pVtxPos[i].mData[0];
		NewPos.y = (float)pVtxPos[i].mData[2];
		NewPos.z = (float)pVtxPos[i].mData[1];
	}

	int Cnt = _pMesh->GetPolygonCount();
}
