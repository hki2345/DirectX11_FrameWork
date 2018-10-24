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
	FbxMatrix	FrameMatrix;
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
	FbxMatrix offsetMatrix;
	std::vector<KeyFrame> KFVec;
};

class Matrial_Info
{
public:
	Matrial_Info() : Diff(KVector::One), Spec(KVector::One), Ambi(KVector::One), Emiv(KVector::One)
	{

	}

public:
	KVector Diff;
	KVector Spec;
	KVector Ambi;
	KVector Emiv;
};


class Material_FbxData
{
public:
	Matrial_Info Info;
	std::wstring MName;
	std::wstring Diff;
	std::wstring Bump;
	std::wstring Spec;
	std::wstring Emiv;
};

// ���� ���� ����ġ�� ��� Ŭ���� 
class WI
{
public:
	int BoneIdx;
	int IndiIdx;
	double dWeight;
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
	~Mesh_FbxData()
	{
		for (size_t i = 0; i < m_MaterialVec.size(); i++)
		{
			delete m_MaterialVec[i];
		}
	}

public:
	std::wstring MName;

	std::vector<KVector> PosVec;
	std::vector<KVector> NormalVec;
	std::vector<KVector> TangentVec;
	std::vector<KVector> BinormalVec;
	std::vector<KVector2> UvVec;

	std::vector<std::vector<KUINT>> IdxVec;
	std::vector<Material_FbxData*> m_MaterialVec;

	bool m_bAni;

	// ��Ű�� ������ ���� �ε����� ������ - ����
	// �޽��� ���Ը� �ּ� �̰� �󸶸�ŭ ������ ���̳Ŀ� ���� ����ġ��
	// ����ϰ� �ȴ�. - 2D �� �ִϸ��̼� �����ϸ� ����

	// ������ �ε����� �ش� �ε����� ����ġ�� ������ Ŭ������ �����Ѵ�.
	std::vector<KVector> IndicesVec;
	std::vector<KVector> WeightsVec;
	std::vector<std::vector<WI>> WIVec;


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
		WIVec.reserve(_Count);
	}
};

class Animation_Info
{
public:
	// �ؿ� �ִϸ��̼��� �ð������� �ɰ��� �����ϰ� �Ǵµ�
	// �׶� ����� �ε��� - �������� ����
	int Index;
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
		for (size_t i = 0; i < MeshData_Vec.size(); i++)
		{
			delete MeshData_Vec[i];
		}
		for (int i = 0; i < AniName_Arr.GetCount(); i++)
		{
			delete AniName_Arr[i];
		}
		for (size_t i = 0; i < Ani_Vec.size(); i++)
		{
			delete Ani_Vec[i];
		}
	}

public:
	// Bone - TRee ������ �Ǿ��ִ�.
	std::vector<KBone*> Bone_Vec;
	std::multimap<std::wstring, KBone*> Bone_Map;

	// Ani
	FbxArray<FbxString*> AniName_Arr;
	std::vector<std::wstring, Animation_Info*> Ani_Map;
	
	// �ִϸ��̼� �����(�ʱⰪ)�� ���� ������ �����ϱ� ����
	std::vector<Animation_Info*> Ani_Vec;
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
	static void Init_FBXLoader();

	// �ݻ� ��Ʈ���� - ������ �ִϸ��̼ǿ�
	// �� ����� ���ϰԵǸ� OFfset�� ���´�.
private:
	static FbxAMatrix m_MReflect;

public:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	KFBX* m_pNewFbx;
	KUINT	m_BoneCnt;

public:
	// �⺻ ���� - �������� Ȯ�� ������ �������� ���� Ȥ�� ����
	void Load_FBX(const wchar_t* _Path);
	void Count_AllBone(FbxNode* _pNode);

	// �� ������
	void Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent);
	
	// �ִϸ��̼� �� �ﰢȭ - �ﰢȭ�� �Ǿ�� �׷�������
	void Check_Animation();
	void Triangulate(FbxNode* _pNode);
	
	// ���� ����
	void Set_Material(FbxNode* _pNode);

	// �޽�
	void Set_MeshData(FbxNode* _pNode);
	void Set_Normal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Tangent(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_BNormal(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);
	void Set_Uv(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData, DWORD _CurIdx, DWORD _CurVtx);

	// ������ ���� �ִϸ��̼� ���� ����
	void Set_AniData(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);
	void Set_WeightIndices(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Set_OffSetMatrix(FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void Set_FrameMatrix(FbxNode* _pNode, FbxCluster* _pC, KBone* _pBone, Mesh_FbxData* _pMeshData);
	void WICheck(FbxMesh* _pMesh, Mesh_FbxData* _pMeshData);

	// 
	KVector Get_MaterialColor(FbxSurfaceMaterial* _pFbxMatData,
		const char* _pMtrlName,
		const char* _pMtrlColorName);

	std::wstring Get_MaterialTexName(FbxSurfaceMaterial* _pFbxMatData,
		const char* _pMtrlTexName);


	// �̰� ���ܷ� ��Ʈ������ �ڵ����� ũ���� ���� �༭ ������ִ� �Լ�
	// ���⿡�� ���� ����� ������ ���� �ǰڴ�.
	FbxAMatrix Get_FbxMatrix(FbxNode* _pNode);
};