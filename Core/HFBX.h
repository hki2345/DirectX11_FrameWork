#pragma once

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



#include <fbxsdk.h>

class HFBX
{
public:
	HFBX();
	~HFBX();

private:
	FbxManager* m_pManager;

public:
	void Load(const wchar_t* _Path);

private:
	void Load_Triangle(FbxNode* _pNode);
	void Load_Mesh(FbxMesh* _pMesh);

};

