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
private:
	static bool m_bInit;
	static FbxManager* m_pManager;
	//static FbxScene* m_pScene;
	//static FbxImporter* m_pImporter; // 파일을 읽어드리기 위한 

private:
	class Des 
	{
	public:
		~Des() {
			HFBX::Destroy();
		}
	};
	friend Des;
	static Des Destroyer;

public:
	static void Destroy();

public:
	static void Init();

public:
	HFBX();
	~HFBX();
};

