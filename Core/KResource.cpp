#include "KResource.h"



KResource::KResource()
{
}


KResource::~KResource()
{
}

// �̰��� ��ӹ��� �ʰ� �����ϸ� ������ ��ȯ�ϰ� �� -> ��� ���� �� �ְ�
bool KResource::Create()
{
	return false;
}
bool KResource::Load()
{
	return false;
}


void KResource::path(const wchar_t* _Value)
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