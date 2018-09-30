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
	// 풀어줄 때 어떤 그룹을 풀어주는지 확정짓기 위해
	// 인덱스를 가지고 있는다.
	LeaveCriticalSection(Get_CS(Index));
}


CRITICAL_SECTION* MultiCritical::Get_CS(int _index)
{
	EnterCriticalSection(&Map_CS);

	std::unordered_map<int, CRITICAL_SECTION*>::iterator findIter = m_CS_Map.find(_index);

	if (m_CS_Map.end() == findIter)
	{
		CRITICAL_SECTION* NewCs = new CRITICAL_SECTION();

		// 마법의 숫자를 막는다 - 힙으로 선언되기에 혹시나하여 0으로 밀어버리는 것
		// 그룹당으로 나눈다하여도 이렇게 개별 초기화는 해주어야 함
		memset(NewCs, 0, sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(NewCs);
		m_CS_Map.insert(std::unordered_map<int, CRITICAL_SECTION*>::value_type(_index, NewCs));
		findIter = m_CS_Map.find(_index);
	}

	CRITICAL_SECTION* thisCs = findIter->second;
	LeaveCriticalSection(&Map_CS);
	return thisCs;
}