#include "HThread.h"
#include "StlHelperFunc.h"

std::unordered_map<std::wstring, HPTR<HThread>> HThread::m_TMap;

HPTR<HThread> HThread::FindThread(const wchar_t* _Name) 
{
	return MapFind<HPTR<HThread>>(m_TMap, _Name);
}

HPTR<HThread> HThread::CreateThread(const wchar_t* _Name)
{
	HThread* NewThread = new HThread();

	m_TMap.insert(std::unordered_map<std::wstring, HPTR<HThread>>::value_type(_Name, NewThread));

	return NewThread;
}

// 접근만 할때는 

bool HThread::StartThread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg)
{
	HPTR<HThread> Ptr = FindThread(_Name);

	if (nullptr == Ptr)
	{
		Ptr = CreateThread(_Name);
	}

	if (true == Ptr->IsWork())
	{
		Ptr->Destroy();
	}

	Ptr->Start(_StdFuncPtr, _Arg);
	return true;
}

unsigned int __stdcall HThread::BaseThreadFunc(void* _Arg)
{
	HThread* pThread = (HThread*)_Arg;

	switch (pThread->m_eMode)
	{
	case FUNC_MODE_STD:
		return pThread->StdFuncPtr(pThread->m_pArg);
		break;
	case FUNC_MODE_MEMBER:
		return pThread->m_MemberPtr(pThread->m_pArg);
		break;
	default:
		break;
	}

	return 0;
}


bool HThread::PauseThread(const wchar_t* _Name) 
{
	HPTR<HThread> Ptr = FindThread(_Name);

	if (nullptr == Ptr)
	{
		TASSERT(true);
	}

	if (true == Ptr->m_bPause)
	{
		return true;
	}

	Ptr->Pause();

	return true;
}

bool HThread::ResumThread(const wchar_t* _Name)
{
	HPTR<HThread> Ptr = FindThread(_Name);

	if (nullptr == Ptr)
	{
		TASSERT(true);
	}

	if (false == Ptr->m_bPause)
	{
		return true;
	}

	Ptr->Resum();

	return true;
} 

HThread::HThread() : m_eMode(FUNC_MODE_NONE), m_Handle(nullptr)
, m_bPause(false), m_pArg(nullptr), StdFuncPtr(nullptr)
, m_MemberPtr(nullptr), m_EndMemberPtr(nullptr), EndStdFuncPtr(nullptr)
{

}
HThread::~HThread() 
{
	// 여기에서 뭔가를 하고 끝내면 되게 만들면 된다.

	if (nullptr != EndStdFuncPtr)
	{
		EndStdFuncPtr(this);
	}

	if (nullptr != m_EndMemberPtr)
	{
		m_EndMemberPtr(this);
	}

	Destroy();
}

bool HThread::Start(unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg)
{
	if (nullptr == _StdFuncPtr)
	{
		TASSERT(true);
		return false;
	}

	m_eMode = FUNC_MODE_STD;

	StdFuncPtr = _StdFuncPtr;
	m_MemberPtr = nullptr;
	StartSetting(_Arg);

	return true;
}


void HThread::StartSetting(void* _Arg)
{
	m_pArg = _Arg;
	m_Handle = (HANDLE)_beginthreadex(nullptr, 0, &HThread::BaseThreadFunc, this, 0, nullptr);
}

void HThread::Pause() 
{
	if ((DWORD)-1 != SuspendThread(m_Handle) )
	{
		m_bPause = true;
		return;
	}
	else {
		int a = 0;
	}


	return;
}

void HThread::Resum() 
{
	if ((DWORD)-1 != ResumeThread(m_Handle))
	{
		m_bPause = false;
		return;
	}

	return;
}

void HThread::Destroy() 
{
	if (nullptr != m_Handle)
	{
		TerminateThread(m_Handle, 0);
		m_Handle = nullptr;
	}
}
//
//void HThread::Create() 
//{
	// 새큐리티 : 자식 프로세스까지 쓰레드로 영향을 미칠때 그에 대한 내용에 대한 구조체를 넣어줄수 있는데. 안씀
	// 스택사이즈 : 스택 크기
	// 함수 포인터
	// 초기인자
	// 시작 옵션 CREATE_SUSPENDED면 시작시 정지한다.
	// 어떤 함수를 한번만 실행하고 정지하는 방식으로 가자.
	// _beginthreadex(nullptr, 0, nullptr, (void*)0, CREATE_SUSPENDED, nullptr);


// }
