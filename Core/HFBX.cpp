#include "HFBX.h"

HFBX::Des HFBX::Destroyer;
bool HFBX::m_bInit = false;
FbxManager* HFBX::m_pManager = nullptr;
//FbxScene* HFBX::m_pScene = nullptr;
//FbxImporter* HFBX::m_pImporter = nullptr; // 파일을 읽어드리기 위한 


HFBX::HFBX()
{
	
}


HFBX::~HFBX()
{
}

void HFBX::Destroy() 
{
	if (nullptr != m_pManager)
	{
		m_pManager->Destroy();
	}
}

void HFBX::Init() 
{
	if (true == m_bInit)
	{
		return;
	}

	m_pManager = FbxManager::Create();
	//m_pScene = nullptr;
	//m_pImporter = nullptr;

	m_bInit = true;
}