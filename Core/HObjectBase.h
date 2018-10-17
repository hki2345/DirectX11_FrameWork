#pragma once
#include "RefBase.h"
#include <string>
#include <typeinfo.h>

class HTypeBase : public HRefBase 
{
protected:
	HTypeBase(const HTypeBase& _Other) : HRefBase(_Other),  m_TypeInfo(_Other.m_TypeInfo), m_bDebug(_Other.m_bDebug) {}
	HTypeBase() : m_bDebug(false) {}
	~HTypeBase() {}

private:
	const type_info* m_TypeInfo;
	bool m_bDebug;

public:
	bool IsDebug() {
		return m_bDebug;
	}

	void DebugOn() 
	{
		m_bDebug = true;
	}

public:
	const type_info* TypeInfo()
	{
		return m_TypeInfo;
	}

	void TypeSetting()
	{
		m_TypeInfo = &typeid(*this);
	}

	template <typename T>
	bool IsEqual()
	{
		return (*m_TypeInfo) == typeid(T);
	}

	template <typename T>
	bool IsParent()
	{
		// return true;

		return dynamic_cast<T*>(this) != nullptr;
	}
};

// type_info
class HObjectBase : public HTypeBase
{
public:
	HObjectBase();
	HObjectBase(const HObjectBase& _Other);
	HObjectBase(const wchar_t* _Name);
	~HObjectBase();

private:
	std::wstring m_Name;

public:
	unsigned int NameSize()
	{
		return (unsigned int)m_Name.size() + 1;
	}

	const wchar_t* Name() 
	{
		return m_Name.c_str();
	}

	void Name(const wchar_t* _Name)
	{
		m_Name = _Name;
	}


};

