#include "KFBX.h"
#include <atlstr.h>
#include "KMacro.h"
#include "DXContainer.h"


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
	pImp->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings());
	pImp->Import(pScene);


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
		BBY;
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
