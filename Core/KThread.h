#pragma once
#include <process.h>
// 이 두해더는 BeginThread시 스레드를 바로 시작할 건지
// 대기하였다가 시작할 건지에 대한 상태를 지정할 때 필요한 해더
#include <Windows.h>
#include <WinBase.h>
#include <unordered_map>
#include <string>
#include "SmartPtr.h"

#include <functional>
#include "KMacro.h"


class KThread : public SmartPtr
{
private:
	enum THREAD_MODE
	{
		TM_NONE,
		TM_STD,
		TM_MEMBER,
	};

private:
	static std::unordered_map<std::wstring, KPtr<KThread>> m_ThreadMap;

public:
	static KPtr<KThread> Find_Thread(const wchar_t* _Name);
	static KPtr<KThread> Create_Thread(const wchar_t* _Name);

	static bool Pause_Thread(const wchar_t* _Name);
	static bool Recov_Thread(const wchar_t* _Name);

	// 일반 전역 함수 시작 -> 바로 스레드에 적용 가능
	static bool Start_Thread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	// 어떤 객체 안 멤버함수를 실행 시켜주는 스레드 -> 형을 맞추어 실행
	template<typename TYPE>
	static bool Start_Thread(const wchar_t* _Name, unsigned int(TYPE::*pMember)(void*), TYPE* _pObj, void* _Arg = nullptr)
	{
		KPtr<KThread> NewPtr = Find_Thread(_Name);
		if (nullptr == NewPtr)
		{
			NewPtr = Create_Thread(_Name);
		}

		if (true == NewPtr->is_run())
		{
			NewPtr->Destroy();
		}

		NewPtr->Start<TYPE>(pMember, _pObj, _Arg);
		return true;
	}

	// 기본적으로 실행될 함수 -> 여기서 스레드가 실행된다.
	static unsigned int __stdcall ThreadFunc(void* _Arg);

public:
	THREAD_MODE m_eMode;

	// 어디에서 어디랑 공유될 건지
	HANDLE m_Handle;

	// 스레드를 일시중지 혹은 시작
	bool m_bPause;

	// 어떤 함수를 실행할지에 대한 포인터
	void* m_pArg;

	// 
	unsigned int(__stdcall *pStdFunc)(void*);
	std::function<unsigned int(void*)> pMember;

	unsigned int(__stdcall *pEndStdFunc)(void*);
	std::function<unsigned int(void*)> pEndMember;


public:
	bool is_run()
	{
		return nullptr != m_Handle;
	}


private:
	bool Start(unsigned int(__stdcall * _StdFuncPtr)(void*), void* _Arg = nullptr);

	template<typename T>
	bool Start(unsigned int(T::*_pMember)(void*), T* _pObj, void* _Arg = nullptr)
	{
		if (nullptr == _pObj || nullptr == _pMember)
		{
			KASSERT(true);
			return false;
		}

		m_eMode = TM_MEMBER;

		pStdFunc = nullptr;
		pMember = bind(_pMember, _pObj, std::placeholders::_1);

		Start_Set(_Arg);

		return true;

	}

private:
	void Start_Set(void* _Arg);

public:
	void Pause();
	void Recov();

private:
	void Destroy();

public:
	KThread();
	~KThread();
};

