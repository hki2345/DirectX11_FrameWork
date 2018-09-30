#include "MultiCritical.h"


bool MultiCritical::bInit = false;
std::unordered_map<int, CRITICAL_SECTION*> MultiCritical::m_CS_Map;
CRITICAL_SECTION MultiCritical::Map_CS = CRITICAL_SECTION();
MultiCritical::Creator Creator_Inst = MultiCritical::Creator();
MultiCritical::Destroy Destroy_Inst = MultiCritical::Destroy();



MultiCritical::MultiCritical(const int& _index)
{
	Index = _index;
	EnterCriticalSection(Get_CS(Index));
}


MultiCritical::~MultiCritical()
{
	// Ǯ���� �� � �׷��� Ǯ���ִ��� Ȯ������ ����
	// �ε����� ������ �ִ´�.
	LeaveCriticalSection(Get_CS(Index));
}


CRITICAL_SECTION* MultiCritical::Get_CS(int _index)
{
	EnterCriticalSection(&Map_CS);

	std::unordered_map<int, CRITICAL_SECTION*>::iterator findIter = m_CS_Map.find(_index);

	if (m_CS_Map.end() == findIter)
	{
		CRITICAL_SECTION* NewCs = new CRITICAL_SECTION();

		// ������ ���ڸ� ���´� - ������ ����Ǳ⿡ Ȥ�ó��Ͽ� 0���� �о������ ��
		// �׷������ �������Ͽ��� �̷��� ���� �ʱ�ȭ�� ���־�� ��
		memset(NewCs, 0, sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(NewCs);
		m_CS_Map.insert(std::unordered_map<int, CRITICAL_SECTION*>::value_type(_index, NewCs));
		findIter = m_CS_Map.find(_index);
	}

	CRITICAL_SECTION* thisCs = findIter->second;
	LeaveCriticalSection(&Map_CS);
	return thisCs;
}