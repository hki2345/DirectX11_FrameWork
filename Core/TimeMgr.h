#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

#include "RefBase.h"

class HCore;
class TimeMgr
{
private:
	friend HCore;

public:
	// 초당 프레임을 계산해 오세요.
	class GameTimer : public HRefBase
	{
	public:
		friend TimeMgr;

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
		GameTimer();
		~GameTimer();

	};

private:
	static GameTimer MainTimer;
	static std::unordered_map<std::wstring, HPTR<GameTimer>> m_MapTimer;

public:
	static HPTR<GameTimer> FindTimer(const wchar_t* _Name);
	static HPTR<GameTimer> CreatTimer(const wchar_t* _Name);

private:
	static void Init();
	static void Update();

public:
	static inline float DeltaTime()
	{
		return MainTimer.m_DeltaTime;
	}

	static inline int FPS()
	{
		return (int)MainTimer.FPS;
	}

private:
	TimeMgr();
	~TimeMgr();
};


