#pragma once
#include "SmartPtr.h"
#include <string>

class TypeBegin : public SmartPtr
{
public:
	TypeBegin() : m_Debug(false) {};
	TypeBegin(const TypeBegin& _Other) :
		SmartPtr(_Other), m_TypeInfo(_Other.m_TypeInfo), m_Debug(_Other.m_Debug) {}
	~TypeBegin() {};

private:
	// 타입의 형을 비교함 -> 여기에 해쉬코드, 이름등이 저장되고
	// 특히 해쉬코드는 컴퓨터가 달라져도 값이 같음 -> 씬을 저장하고
	// 불러오는데에 중요한 역할
	const type_info* m_TypeInfo;
	bool m_Debug;

public:
	bool& is_debug() { return m_Debug; }
	void debug_On() { m_Debug = true; }
	void debug_Off() { m_Debug = false; }
	
	const type_info* typeinfo()
	{
		return m_TypeInfo;
	}

	void set_type()
	{
		// 타입 인포를 저장하는 단계 -> 인자값으로는 값형식을 넣어준다.
		// 포인터를 그대로 넣어주면 값 비교시 포인터를 비교하는 꼴이 되어버림
		m_TypeInfo = &typeid(*this);
	}

	// 여러 타입을 비교할 수 있게끔 탬플릿으로 받게 한다.
	template <typename T>
	bool is_equal()
	{
		return (*m_TypeInfo) == typeid(T);
	}


	// 다이나믹 캐스트 -> virtual로 된 클래스 간 상속의 여부를 알수 있음
	// 정확히 말하면 상속이 이루어져 있을 때 그 상속하는 객체의 주소를 가져옴
	// 다운 캐스팅은 안되며 virtual 연결이 되어야만 함
	template <typename T>
	bool is_parent()
	{
		return dynamic_cast<T*>(this) != nullptr;
	}
};

class Begin : public TypeBegin
{
public:
	Begin();
	Begin(const wchar_t* _Name);
	Begin(const Begin& _Other);
	~Begin();

private:
	std::wstring m_Name;

public:
	unsigned int name_size() const
	{
		return (unsigned int)m_Name.size() + 1;
	}

	const wchar_t* name()
	{
		return m_Name.c_str();
	}

	const std::wstring ws_name()
	{
		return m_Name;
	}

	void name(const wchar_t* _Name)
	{
		m_Name = _Name;
	}
};

