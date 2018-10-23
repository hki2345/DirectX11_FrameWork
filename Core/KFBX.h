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

	// ��Ű�� ���� - 
	KVector m_Indices;
	// ����ġ ���� - �ִϸ��̼� ��ȯ �� ���� Ȥ�� ���� �ִϸ��̼��� �󸶳� ���� ���̳�
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

	// ��Ű�� ������ ���� �ε����� ������ - ����
	// �޽��� ���Ը� �ּ� �̰� �󸶸�ŭ ������ ���̳Ŀ� ���� ����ġ��
	// ����ϰ� �ȴ�. - 2D �� �ִϸ��̼� �����ϸ� ����
	std::vector<KVector> IndicesVec;
	std::vector<KVector> WeightsVec;


	// ������� �������� �ٸ�����.
	// ������� Ȯ���ϴ� �����̶��, ��������� ���� �ڴ� ��
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
	// Bone - TRee ������ �Ǿ��ִ�.
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
	// �⺻ ���� - �������� Ȯ�� ������ �������� ���� Ȥ�� ����
	void Load(const wchar_t* _Path);
	void Count_AllBone(FbxNode* _pNode);

	// �� ������
	void Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent);
	
	// �ִϸ��̼�
	void Check_Animation();
	void Triangulate(FbxNode* _pNode);
	
	// �޽�
	void Set_MeshData(FbxNode* _pNode);
	void Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);

	// ������ ���� �ִϸ��̼� ���� ����
	void Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);
	void Set_WeightIndices();
	void Set_OffSetMat();
	void Set_FrameMat();


	// �̰� ���ܷ� ��Ʈ������ �ڵ����� ũ���� ���� �༭ ������ִ� �Լ�
	// ���⿡�� ���� ����� ������ ���� �ǰڴ�.
	FbxAMatrix Get_FbxMatrix(FbxNode* _pNode);
};