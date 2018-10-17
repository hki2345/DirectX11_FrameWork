#include "HRes.h"



HRes::HRes()
{
}

HRes::HRes(const HRes& _Other) : HObjectBase(_Other),
	m_PathKey(_Other.m_PathKey),
	m_Path(_Other.m_Path),
	m_FullFileName(_Other.m_FullFileName),
	m_FileName(_Other.m_FileName),
	m_Ext(_Other.m_Ext)

{

}

HRes::~HRes()
{
}


bool HRes::Create()
{
	return false;
}
bool HRes::Load() 
{
	return false;
}

void HRes::Path(const wchar_t* _Path) 
{
	wchar_t ArrDrive[128];
	wchar_t ArrFolder[128];
	wchar_t ArrFile[128];
	wchar_t ArrExt[128];
	_wsplitpath_s(_Path, ArrDrive, ArrFolder, ArrFile, ArrExt);

	std::wstring PathKey = ArrFolder;
	m_Path = _Path;
	m_FileName = ArrFile;
	m_Ext = ArrExt; 

	m_FullFileName = m_FileName + m_Ext;
}