#include "KResource.h"



KResource::KResource()
{
}
KResource::KResource(const KResource& _Other) : 
	Begin(_Other),
	m_sForderPath(_Other.m_sForderPath),
	m_sPath(_Other.m_sPath),
	m_sNameExi(_Other.m_sNameExi),
	m_sFileName(_Other.m_sFileName),
	m_Exist(_Other.m_Exist)
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