#include "Resource.h"



Resource::Resource()
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


void Resource::path(const wchar_t* _Value)
{
	wchar_t ArrDrive[128];
	wchar_t ArrFolder[128];
	wchar_t ArrFile[128];
	wchar_t ArrExist[128];

	// ���� ��θ� ��´�.
	_wsplitpath_s(_Value, ArrDrive, ArrFolder, ArrFile, ArrExist);
	m_sPath = _Value;
	m_sFileName = ArrFile;
	m_Exist = ArrExist;

	m_sNameExi = m_sFileName + m_Exist;
}