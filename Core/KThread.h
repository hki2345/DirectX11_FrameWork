#pragma once
#include <process.h>
// �� ���ش��� BeginThread�� �����带 �ٷ� ������ ����
// ����Ͽ��ٰ� ������ ������ ���� ���¸� ������ �� �ʿ��� �ش�
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

	// �Ϲ� ���� �Լ� ���� -> �ٷ� �����忡 ���� ����
	static bool Start_Thread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	// � ��ü �� ����Լ��� ���� �����ִ� ������ -> ���� ���߾� ����
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

	// �⺻������ ����� �Լ� -> ���⼭ �����尡 ����ȴ�.
	static unsigned int __stdcall ThreadFunc(void* _Arg);

public:
	THREAD_MODE m_eMode;

	// ��𿡼� ���� ������ ����
	HANDLE m_Handle;

	// �����带 �Ͻ����� Ȥ�� ����
	bool m_bPause;

	// � �Լ��� ���������� ���� ������
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

