#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

#include "Begin.h"

#define DELTATIME TimeManager::DeltaTime()


// 스레드를 사용할 시 QueryPerformanceFrequency() 
// 이 함수는 CPU의 클럭과 밀접한 관련이 있는데 일반적인 경우에는 
// 멀티 프로세서 환경에서 어떤 프로세서에서 호출되는지에 관계없이
// 정상적으로 동작하지만 BIOS에 버그가 있는 경우에는 호출되는
// 프로세서에 따라서 다른 값이 리턴되는 경우가 있다.

// GetProcessAffinityMask() 그걸 이 녀석이 보정해 주는데...
// 함수 자체는 설정된 커널을 이용하여 거기서만 실행되게 한다.
// 하지만 CPU에 따라 다른 함수가 해당 커널을 이미 점유하고 있다면
// 커널 설정이 불가능 -> 따라서 유연하게 설정하기 위한 클래스
class SingleCore
{
public:
	class SingleCoreError
	{
	};

	ULONG_PTR oam_;

	SingleCore()
	{
		ULONG_PTR pam, sam;
		if (!GetProcessAffinityMask(GetCurrentProcess(), &pam, &sam))
			throw SingleCoreError();

		ULONG_PTR am = 1;
		int bits = CHAR_BIT * sizeof(am);

		for (int i = 0; i<bits; ++i)
		{
			if (am & pam)
			{
				oam_ = SetThreadAffinityMask(GetCurrentThread(), am);
				if (!oam_)
					throw SingleCoreError();

				break;
			}

			am <<= 1;
		}
	}

	~SingleCore()
	{
		SetThreadAffinityMask(GetCurrentThread(), oam_);
	}
};


// Read more : http://www.jiniya.net/wp/archives/7647#ixzz5MPjZEyio

class TimeManager
{
private:
	friend class KCore;

	// 긁어오셨네요ㅋㅋ
public:
	// 초당 프레임을 계산해 오세요.
	class Timer : public Begin
	{
	public:
		friend TimeManager;

	private:
		LARGE_INTEGER m_SecCount; // cpu 가 1초에 몇번 숫자를 세느냐.
		LARGE_INTEGER m_CurCount;
		LARGE_INTEGER m_NextCount;
		float m_DeltaTime;
		float AccFPS;
		int FPSCount;
		int FPS;

	public:
		void Init();
		float Update();

	public:
		Timer();
		~Timer();

	};

private:
	// 요부분 추가 -> 해쉬로 타이머를 여러개 둔다.
	static Timer this_Timer;
	static std::unordered_map<std::wstring, KPtr<Timer>> m_MapTimer;
	
public:
	static KPtr<Timer> Find_Timer(const wchar_t* _Name);
	static KPtr<Timer> Create_Timer(const wchar_t* _Name);

private:
	static void Init();
	static void Update();

public:
	static inline float DeltaTime()
	{
		return this_Timer.m_DeltaTime;
	}

	static inline int FPS()
	{
		return (int)this_Timer.FPS;
	}


public:
	TimeManager();
	~TimeManager();
};

