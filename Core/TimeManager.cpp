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

	// 절대 바귀지 않는 cpu의 타임을 알아오는 함수
	QueryPerformanceFrequency(&m_SecCount);

	// 누적타임 
	QueryPerformanceCounter(&m_CurCount);
	AccFPS = 0;
	FPSCount = 0;
}

float TimeManager::Timer::Update()
{
	// 스레드에 불안정한 커널진입을 막기위한 함수 실행하는 클래스
	SingleCore s;

	// 델타타임을 구한다.
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

	// 만드는 것과 찾는 것을 같이 해주는 역할
	if (nullptr != NewPtr)
	{
		return NewPtr;
	}

	TimeManager::Timer* NewTimer = new TimeManager::Timer();
	m_MapTimer.insert(std::unordered_map<std::wstring, KPtr<Timer>>::value_type(_Name, NewTimer));

	return NewTimer;
}