#pragma once
#include "KMacro.h"
#include <fbxsdk.h>
#include <vector>
#include <map>
#include <string>


#if X64
#if _DEBUG
#pragma comment (lib, "libfbxsdk-md_D64.lib")
#else if NDEBUG
#pragma comment (lib, "libfbxsdk-md_R64.lib")
#endif
#else if WIN32
#if _DEBUG
#pragma comment (lib, "libfbxsdk-md_D86.lib")
#else if NDEBUG
#pragma comment (lib, "libfbxsdk-md_R86.lib")
#endif
#endif // DEBUG


class KeyFrame
{
public:
	FbxMatrix	BoneMat;
	double		KeyTime;
};

class KBone
{
public:
	std::wstring Name;
	KUINT Depth;
	KUINT Index;
	KBone* m_pPBone;
	FbxMatrix Offset;
	FbxMatrix BoneMat;
	std::vector<KeyFrame> KFVec;
};

class Matrix_FbxData
{

};

class Vertex_FbxData
{
public:
	std::wstring Nmae;
	KVector m_Pos;
	KVector m_Normal;
	KVector m_Tangent;
	KVector m_Binormal;
	KVector2 m_UV;

	// 스키닝 정보 - 
	KVector m_Indices;
	// 가중치 정보 - 애니메이션 전환 간 이전 혹은 다음 애니메이션을 얼마나 섞을 것이냐
	KVector m_Weights;

	bool m_bAni;
};

class Mesh_FbxData
{
public:
	std::wstring MName;

	std::vector<KVector> PosVec;
	std::vector<KVector> NormalVec;
	std::vector<KVector> TangentVec;
	std::vector<KVector> BinormalVec;
	std::vector<KVector2> UvVec;

	std::vector<std::vector<KUINT>> IdxVec;

	Matrix_FbxData m_MatData;

	bool m_bAni;

	// 스키닝 정보에 대한 인덱스의 복수형 - 지수
	// 메쉬도 무게를 둬서 이게 얼마만큼 움직일 것이냐에 대한 가중치를
	// 계산하게 된다. - 2D 본 애니메이션 생각하면 쉬움
	std::vector<KVector> IndicesVec;
	std::vector<KVector> WeightsVec;


	// 리저브와 리사이즌 다르ㄴ다.
	// 리저브는 확보하는 개념이라면, 리사이즈는 때려 박는 식
	void Set_VertexCount(const KUINT& _Count)
	{
		PosVec.reserve(_Count);
		NormalVec.reserve(_Count);
		TangentVec.reserve(_Count);
		BinormalVec.reserve(_Count);
		UvVec.reserve(_Count);
		IndicesVec.reserve(_Count);
		WeightsVec.reserve(_Count);
	}
};

class Animation_Info
{
public:
	std::wstring Name;
	FbxTime Stime;
	FbxTime Etime;
	FbxLongLong Length_Time;
	FbxTime::EMode eMode;
};

class KFBX
{
public:
	~KFBX()
	{
		for (size_t i = 0; i < Bone_Vec.size(); i++)
		{
			delete Bone_Vec[i];
		}
		for (size_t i = 0; i < Bone_Vec.size(); i++)
		{
			delete MeshData_Vec[i];
		}
	}

public:
	// Bone - TRee 구조로 되어있다.
	std::vector<KBone*> Bone_Vec;
	std::multimap<std::wstring, KBone*> Bone_Map;

	// Ani
	FbxArray<FbxString*> AniName_Arr;
	std::map<std::wstring, Animation_Info*> Ani_Map;
	std::vector<Mesh_FbxData*> MeshData_Vec;

public:
	KBone* Find_Bone(const wchar_t* _Name);
};


class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

public:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	KFBX* m_pNewFbx;
	KUINT	m_BoneCnt;

public:
	// 기본 세팅 - 데이터의 확인 유무와 정상적인 파일 혹은 보정
	void Load(const wchar_t* _Path);
	void Count_AllBone(FbxNode* _pNode);

	// 본 얻어오기
	void Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent);
	
	// 애니메이션
	void Check_Animation();
	void Triangulate(FbxNode* _pNode);
	
	// 메쉬
	void Set_MeshData(FbxNode* _pNode);
	void Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);

	// 정점에 대한 애니메이션 관련 정보
	void Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);
	void Set_WeightIndices();
	void Set_OffSetMat();
	void Set_FrameMat();


	// 이건 번외로 메트릭스를 자동으로 크자이 곱해 줘서 계산해주는 함수
	// 여기에서 나온 행렬을 보정해 쓰면 되겠다.
	FbxAMatrix Get_FbxMatrix(FbxNode* _pNode);
};