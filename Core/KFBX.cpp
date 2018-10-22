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
	pImp->Initialize(CW2A(_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings());
	pImp->Import(pScene);


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
		BBY;
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
