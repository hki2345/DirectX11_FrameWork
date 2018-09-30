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

	// �����带 �׷����� �����ϸ� �� �׷� �ϳ��ϳ��Ͽ� ����
	// �ʱ�ȭ�� �ʿ��� �κ��̹Ƿ� ũ��Ƽ�ü����� ���������� �ϳ��� �������ִ�.
	// ���� ������ �׷�� �����Ǵ� ���̸� �� Ű�� ��� �ִ� �����尡 ������
	// �������� ��⸦ Ÿ���Ѵ�.
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

