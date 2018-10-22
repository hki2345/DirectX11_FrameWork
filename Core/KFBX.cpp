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

	Load_Bone(m_pScene->GetRootNode(), 0, nullptr);
	Check_Animation();
	Triangulate(m_pScene->GetRootNode());

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
	// 전역으로 선언해 어디서든 볼 수 있게 함ㅇㅇ
	m_pManager = FbxManager::Create();;

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
	FbxScene* pScene = FbxScene::Create(m_pManager, "");
	FbxImporter* pImp = FbxImporter::Create(m_pManager, "");

	// m_psz -> 자동 string 변환 -> wstring에서 char* 형으로
	bool Temp = pImp->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings());
	bool Temp2 = pImp->Import(pScene);


	Load_Triangle(pScene->GetRootNode());

	pImp->Destroy();
	pScene->Destroy();
	pIos->Destroy();
	m_pManager->Destroy();
}


// FBX라 해도 트리구조로 되어있다.
void KFBX::Load_Triangle(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (nullptr == pAttr)
	{
		// BBY;
	}

	if (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		// 우리 쪽에서 쓰기 쉽게 변경해주는 작업
		// 일종의 컨버트 하는 과정이고 메쉬정보를 추출할 수 있다.
		FbxGeometryConverter NewCon(m_pManager);

		// 2번째 bool은 덮어 쓸거냐 말거냐를 묻는다.
		NewCon.Triangulate(pAttr, true);

		FbxMesh* NewMesh = _pNode->GetMesh();
		Load_Mesh(NewMesh);
	}



	int MatCnt = _pNode->GetMaterialCount();

	if (MatCnt > 0)
	{
		// 메터리얼 정보 로드
	}


	// 트리구조 -> 자식이 있으면 그걸 제귀돌려서 가져옴
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


	// FBX에서 제공하는 Vector는 x, z, y 순이다.
	for (int i = 0; i < iVtxCnt; i++)
	{
		NewPos.x = (float)pVtxPos[i].mData[0];
		NewPos.y = (float)pVtxPos[i].mData[2];
		NewPos.z = (float)pVtxPos[i].mData[1];
	}

	int Cnt = _pMesh->GetPolygonCount();
}
