#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

#include "Begin.h"

#define DELTATIME TimeManager::DeltaTime()


// �����带 ����� �� QueryPerformanceFrequency() 
// �� �Լ��� CPU�� Ŭ���� ������ ������ �ִµ� �Ϲ����� ��쿡�� 
// ��Ƽ ���μ��� ȯ�濡�� � ���μ������� ȣ��Ǵ����� �������
// ���������� ���������� BIOS�� ���װ� �ִ� ��쿡�� ȣ��Ǵ�
// ���μ����� ���� �ٸ� ���� ���ϵǴ� ��찡 �ִ�.

// GetProcessAffinityMask() �װ� �� �༮�� ������ �ִµ�...
// �Լ� ��ü�� ������ Ŀ���� �̿��Ͽ� �ű⼭�� ����ǰ� �Ѵ�.
// ������ CPU�� ���� �ٸ� �Լ��� �ش� Ŀ���� �̹� �����ϰ� �ִٸ�
// Ŀ�� ������ �Ұ��� -> ���� �����ϰ� �����ϱ� ���� Ŭ����
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

	// �ܾ���̳׿䤻��
public:
	// �ʴ� �������� ����� ������.
	class Timer : public Begin
	{
	public:
		friend TimeManager;

	private:
		LARGE_INTEGER m_SecCount; // cpu �� 1�ʿ� ��� ���ڸ� ������.
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
	// ��κ� �߰� -> �ؽ��� Ÿ�̸Ӹ� ������ �д�.
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

