#pragma once
#include "SmartPtr.h"
#include <string>

class TypeBegin : public SmartPtr
{
private:
	// Ÿ���� ���� ���� -> ���⿡ �ؽ��ڵ�, �̸����� ����ǰ�
	// Ư�� �ؽ��ڵ�� ��ǻ�Ͱ� �޶����� ���� ���� -> ���� �����ϰ�
	// �ҷ����µ��� �߿��� ����
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
		// Ÿ�� ������ �����ϴ� �ܰ� -> ���ڰ����δ� �������� �־��ش�.
		// �����͸� �״�� �־��ָ� �� �񱳽� �����͸� ���ϴ� ���� �Ǿ����
		m_TypeInfo = &typeid(*this);
	}

	// ���� Ÿ���� ���� �� �ְԲ� ���ø����� �ް� �Ѵ�.
	template <typename T>
	bool is_equal()
	{
		return (*m_TypeInfo) == typeid(T);
	}


	// ���̳��� ĳ��Ʈ -> virtual�� �� Ŭ���� �� ����� ���θ� �˼� ����
	// ��Ȯ�� ���ϸ� ����� �̷���� ���� �� �� ����ϴ� ��ü�� �ּҸ� ������
	// �ٿ� ĳ������ �ȵǸ� virtual ������ �Ǿ�߸� ��
	template <typename T>
	bool is_parent()
	{
		return dynamic_cast<T*>(this) != nullptr;
	}

public:
	TypeBegin() : m_Debug(false) {};
	~TypeBegin() {};
};

class Begin : public TypeBegin
{
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
public:
	Begin();
	Begin(const wchar_t* _Name);
	~Begin();
};

