#pragma once
#include <process.h>
#include <Windows.h>
#include <unordered_map>
#include <string>
#include "SmartPtr.h"
#include <WinBase.h>
#include <functional>
#include "KMacro.h"

class HThread : public SmartPtr
{
private:
	enum FUNC_MODE
	{
		FUNC_MODE_NONE,
		FUNC_MODE_STD,
		FUNC_MODE_MEMBER,
	};

	// 쓰

private:
	static std::unordered_map<std::wstring, KPtr<HThread>> m_TMap;

public:
	static KPtr<HThread> FindThread(const wchar_t* _Name);
	static KPtr<HThread> CreateThread(const wchar_t* _Name);
	static bool PauseThread(const wchar_t* _Name);
	static bool ResumThread(const wchar_t* _Name);

	static bool StartThread(const wchar_t* _Name, unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	template<typename TYPE>
	static bool StartThread(const wchar_t* _Name, unsigned int(TYPE::*pMember)(void*), TYPE* _pObj, void* _Arg = nullptr)
	{
		KPtr<HThread> Ptr = FindThread(_Name);
		if (nullptr == Ptr)
		{
			Ptr = CreateThread(_Name);
		}

		if (true == Ptr->IsWork())
		{
			Ptr->Destroy();
		}

		Ptr->Start<TYPE>(pMember, _pObj, _Arg);
		return true;
	}


	static unsigned int __stdcall BaseThreadFunc(void* _Arg);

////////////////////// 맴버

private:
	FUNC_MODE m_eMode;

	HANDLE m_Handle;
	bool m_bPause;
	void* m_pArg;

	// 전역으로 할때.
	unsigned int(__stdcall *StdFuncPtr)(void*);
	std::function<unsigned int(void*)> m_MemberPtr;

	unsigned int(__stdcall *EndStdFuncPtr)(void*);
	std::function<unsigned int(void*)> m_EndMemberPtr;

public:
	bool IsWork() {
		return nullptr != m_Handle;
	}

private:
	bool Start(unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	template<typename T>
	bool Start(unsigned int(T::*pMember)(void*), T* _pObj, void* _Arg = nullptr)
	{
		if (nullptr == _pObj || nullptr == pMember)
		{
			KASSERT(true);
			return false;
		}

		m_eMode = FUNC_MODE_MEMBER;

		StdFuncPtr = nullptr;
		m_MemberPtr = bind(pMember, _pObj, std::placeholders::_1);

		StartSetting(_Arg);

		return true;
	}

private:
	void StartSetting(void* _Arg);

public:
	void Pause();
	void Resum();

private:
	void Destroy();

private:
	HThread();
	~HThread();
};

