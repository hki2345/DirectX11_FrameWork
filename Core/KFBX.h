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
	std::vector<Vertex_FbxData> m_VDVec;
	Matrix_FbxData m_MatIfo;
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
	}

public:
	// Bone - TRee ������ �Ǿ��ִ�.
	std::vector<KBone*> Bone_Vec;
	std::multimap<std::wstring, KBone*> Bone_Map;

	// Ani
	FbxArray<FbxString*> AniName_Arr;
	std::map<std::wstring, Animation_Info*> Ani_Map;
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
	void Load(const wchar_t* _Path);
	void Count_AllBone(FbxNode* _pNode);
	void Load_Bone(FbxNode* _pNode, KUINT _Depth, KBone* _pParent);

	void Check_Animation();
	void Triangulate(FbxNode* _pNode);
	void Set_MeshData(FbxMesh* _pMesh);
};