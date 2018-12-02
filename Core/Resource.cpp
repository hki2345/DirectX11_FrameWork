#include "Resource.h"



Resource::Resource()
{
}

Resource::Resource(const Resource& _Other) : Begin(_Other),
	m_FileForder(_Other.m_FileForder),
	m_AllPath(_Other.m_AllPath),
	m_FileNameExi(_Other.m_FileNameExi),
	m_FileName(_Other.m_FileName),
	m_FileExt(_Other.m_FileExt)

{

}

Resource::~Resource()
{
}


// �̰��� ��ӹ��� �ʰ� �����ϸ� ������ ��ȯ�ϰ� �� -> ��� ���� �� �ְ�
bool Resource::Create()
{
	return false;
}
bool Resource::Load() 
{
	return false;
}
bool Resource::Save(KPtr<Resource> _Res)
{
	return false;
}


void Resource::Split_Path(const wchar_t* _Path) 
{
	m_AllPath = _Path;

	wchar_t ArrDrive[256];
	wchar_t ArrFolder[256];
	wchar_t ArrFile[256];
	wchar_t ArrExt[256];
	_wsplitpath_s(_Path, ArrDrive, ArrFolder, ArrFile, ArrExt);

	// m_FileForder = ArrDrive;
	// m_FileForder += ArrFolder;

	m_FileName = ArrFile;
	m_FileExt = ArrExt; 
	m_FileNameExi = m_FileName + m_FileExt;

	// ���� �� ������ - ��Ƽ
	// ������ ���� �̸����� �Ǵ� ���� - �ַ�
	if (L"" == m_FileForder)
	{
		m_FileForder = m_FileNameExi;
	}
}