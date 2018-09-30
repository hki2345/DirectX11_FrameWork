#include "KThread.h"
#include "Stl_Assistor.h"

std::unordered_map<std::wstring, KPtr<KThread>> KThread::m_ThreadMap;

KThread::KThread() : m_eMode(TM_NONE), m_Handle(nullptr)
, m_bPause(false), m_pArg(nullptr), pStdFunc(nullptr)
, pMember(nullptr)
{
}


KThread::~KThread()
{
	if (nullptr != pEndMember)
	{
		pEndStdFunc(this);
	}

	if (nullptr != pEndStdFunc)
	{
		pEndStdFunc(this);
	}

	Destroy();
}

KPtr<KThread> KThread::Find_Thread(const wchar_t* _Name)
{
	return Map_Find<KPtr<KThread>>(m_ThreadMap, _Name);
}

KPtr<KThread> KThread::Create_Thread(const wchar_t* _Name)
{
	KThread* NewThread = new KThread();

	m_ThreadMap.insert(std::unordered_map<std::wstring, KPtr<KThread>>::value_type(_Name, NewThread));

	return NewThread;
}




bool KThread::Start_Thread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg)
{
	KPtr<KThread> NewThread = Find_Thread(_Name);

	if (nullptr == NewThread)
	{
		NewThread = Create_Thread(_Name);
	}

	if (true == NewThread->is_run())
	{
		NewThread->Destroy();
	}

	NewThread->Start(_StdFuncPtr, _Arg);
	return true;
}

unsigned int __stdcall KThread::ThreadFunc(void* _Arg)
{
	KThread* pThread = (KThread*)_Arg;


	// 스레드가 전역인지 멤버인지에 따라 구분해서 실행
	switch (pThread->m_eMode)
	{
	case TM_STD:
		return pThread->pStdFunc(pThread->m_pArg);
		break;
	case TM_MEMBER:
		return pThread->pMember(pThread->m_pArg);
		break;
	default:
		break;
	}

	return 0;
}


bool KThread::Pause_Thread(const wchar_t* _Name)
{
	KPtr<KThread> NewThread = Find_Thread(_Name);

	if (nullptr == NewThread)
	{
		KASSERT(true);
	}

	if (true == NewThread->m_bPause)
	{
		return true;
	}

	NewThread->Pause();

	return true;
}

bool KThread::Recov_Thread(const wchar_t* _Name)
{
	KPtr<KThread> NewThread = Find_Thread(_Name);

	if (nullptr == NewThread)
	{
		KASSERT(true);
	}

	if (false == NewThread->m_bPause)
	{
		return true;
	}

	NewThread->Recov();

	return true;
}


bool KThread::Start(unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg)
{
	if (nullptr == _StdFuncPtr)
	{
		KASSERT(true);
		return false;
	}

	m_eMode = TM_STD;

	pStdFunc = _StdFuncPtr;
	pMember = nullptr;
	Start_Set(_Arg);

	return true;
}


void KThread::Start_Set(void* _Arg)
{
	m_pArg = _Arg;
	m_Handle = (HANDLE)_beginthreadex(nullptr, 0, &KThread::ThreadFunc, this, 0, nullptr);
}

void KThread::Pause()
{
	if ((DWORD)-1 != SuspendThread(m_Handle))
	{
		m_bPause = true;
		return;
	}


	return;
}

void KThread::Recov()
{
	if ((DWORD)-1 != ResumeThread(m_Handle))
	{
		m_bPause = false;
		return;
	}

	return;
}

void KThread::Destroy()
{
	if (nullptr != m_Handle)
	{
		TerminateThread(m_Handle, 0);
		m_Handle = nullptr;
	}
}