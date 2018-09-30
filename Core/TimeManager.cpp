#include "TimeManager.h"
#include "Stl_Assistor.h"

/********************** Time ************************/

TimeManager::Timer TimeManager::this_Timer = Timer();
std::unordered_map<std::wstring, KPtr<TimeManager::Timer>> TimeManager::m_MapTimer;

TimeManager::Timer::Timer()
{
	Init();
}
TimeManager::Timer::~Timer()
{
}

void TimeManager::Timer::Init()
{

	// ���� �ٱ��� �ʴ� cpu�� Ÿ���� �˾ƿ��� �Լ�
	QueryPerformanceFrequency(&m_SecCount);

	// ����Ÿ�� 
	QueryPerformanceCounter(&m_CurCount);
	AccFPS = 0;
	FPSCount = 0;
}

float TimeManager::Timer::Update()
{
	// �����忡 �Ҿ����� Ŀ�������� �������� �Լ� �����ϴ� Ŭ����
	SingleCore s;

	// ��ŸŸ���� ���Ѵ�.
	QueryPerformanceCounter(&m_NextCount);

	m_DeltaTime = ((float)(m_NextCount.QuadPart - m_CurCount.QuadPart)) / ((float)m_SecCount.QuadPart);
	m_CurCount.QuadPart = m_NextCount.QuadPart;

	AccFPS += m_DeltaTime;

	++FPSCount;

	if (1.0f <= AccFPS)
	{
		FPS = (int)((float)FPSCount / AccFPS);
		AccFPS = 0.0f;
		FPSCount = 0;
	}

	return m_DeltaTime;
}

/********************** TImeManager ************************/

TimeManager::TimeManager()
{
}


TimeManager::~TimeManager()
{

}

void TimeManager::Init()
{
	this_Timer.Init();
}

void TimeManager::Update()
{
	this_Timer.Update();
}


KPtr<TimeManager::Timer> TimeManager::Find_Timer(const wchar_t* _Name)
{
	return Map_Find<KPtr<TimeManager::Timer>>(m_MapTimer, _Name);
}
KPtr<TimeManager::Timer> TimeManager::Create_Timer(const wchar_t* _Name)
{
	KPtr<TimeManager::Timer> NewPtr = Find_Timer(_Name);

	// ����� �Ͱ� ã�� ���� ���� ���ִ� ����
	if (nullptr != NewPtr)
	{
		return NewPtr;
	}

	TimeManager::Timer* NewTimer = new TimeManager::Timer();
	m_MapTimer.insert(std::unordered_map<std::wstring, KPtr<Timer>>::value_type(_Name, NewTimer));

	return NewTimer;
}