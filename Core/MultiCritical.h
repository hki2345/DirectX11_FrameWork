#pragma once
#pragma once
#include <Windows.h>
#include <unordered_map>
#include <process.h>

class MultiCritical
{
public:
	class Creator { public: Creator() { MultiCritical::Init(); } };
	class Destroy { public: Destroy() { MultiCritical::Release(); } };


private:
	static bool bInit;

	// 스레드를 그룹으로 관리하며 그 그룹 하나하나하에 대한
	// 초기화도 필요한 부분이므로 크리티컬섹션이 개별적으로 하나씩 가지고있다.
	// 개별 색션은 그룹당 공유되는 식이며 그 키를 쥐고 있는 스레드가 있으면
	// 나머지는 대기를 타야한다.
	static std::unordered_map<int, CRITICAL_SECTION*> m_CS_Map;
	static CRITICAL_SECTION Map_CS;
	static Creator Creator_Inst;
	static Destroy Destroy_Inst;


	int Index;
	CRITICAL_SECTION* Get_CS(int _index);
	

public:
	static void Init()
	{
		InitializeCriticalSection(&Map_CS);
		bInit = true;
	}

	static void Release()
	{
		std::unordered_map<int, CRITICAL_SECTION*>::iterator S_CSIter = m_CS_Map.begin();
		std::unordered_map<int, CRITICAL_SECTION*>::iterator E_CSIter = m_CS_Map.end();
		for (; S_CSIter != E_CSIter; ++S_CSIter)
		{
			delete S_CSIter->second;
		}
	}

public:
	MultiCritical(const int& _index);
	~MultiCritical();
};

