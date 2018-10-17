#include "TimeMgr.h"
#include "StlHelperFunc.h"

TimeMgr::GameTimer TimeMgr::MainTimer = GameTimer();
std::unordered_map<std::wstring, HPTR<TimeMgr::GameTimer>> TimeMgr::m_MapTimer;

TimeMgr::GameTimer::GameTimer()
{
	Init();
}
TimeMgr::GameTimer::~GameTimer()
{

}



void TimeMgr::GameTimer::Init()
{
	// 절대 바귀지 않는 cpu의 타임을 알아오는 함수
	QueryPerformanceFrequency(&m_SecCount);

	// 누적타임 
	QueryPerformanceCounter(&m_CurCount);
	AccFPS = 0;
	FPSCount = 0;
}

void TimeMgr::Init() 
{
	MainTimer.Init();
}

HPTR<TimeMgr::GameTimer> TimeMgr::FindTimer(const wchar_t* _Name) 
{
	return MapFind<HPTR<TimeMgr::GameTimer>>(m_MapTimer, _Name);
}

HPTR<TimeMgr::GameTimer> TimeMgr::CreatTimer(const wchar_t* _Name) 
{
	HPTR<TimeMgr::GameTimer> PTR = FindTimer(_Name);

	if (nullptr != PTR)
	{
		return PTR;
	}

	TimeMgr::GameTimer* NewTimer = new TimeMgr::GameTimer();
	m_MapTimer.insert(std::unordered_map<std::wstring, HPTR<GameTimer>>::value_type(_Name, NewTimer));

	return NewTimer;
}

float TimeMgr::GameTimer::Update()
{
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

TimeMgr::TimeMgr()
{
}


TimeMgr::~TimeMgr()
{
}


void TimeMgr::Update()
{
	MainTimer.Update();
}
