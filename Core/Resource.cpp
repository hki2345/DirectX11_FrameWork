#include "Resource.h"



Resource::Resource()
{
}


Resource::~Resource()
{
}

// 이것을 상속받지 않고 실행하면 거짓을 반환하게 함 -> 상속 받을 수 있게
bool Resource::Create()
{
	return false;
}
bool Resource::Load()
{
	return false;
}


void Resource::path(const wchar_t* _Value)
{
	wchar_t ArrDrive[128];
	wchar_t ArrFolder[128];
	wchar_t ArrFile[128];
	wchar_t ArrExist[128];

	// 파일 경로를 뜯는다.
	_wsplitpath_s(_Value, ArrDrive, ArrFolder, ArrFile, ArrExist);
	m_sPath = _Value;
	m_sFileName = ArrFile;
	m_Exist = ArrExist;

	m_sNameExi = m_sFileName + m_Exist;
}