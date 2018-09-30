#pragma once
#include <type_traits>

template<typename BASE, typename CHILD>
class TypeCheck
{
private:
	static short Check(BASE*) {  };
	static char Check(...) {  };

	static CHILD* p;

public:
	// sizeof Ʈ�� -> sizeof�� �������̱� ������ �̰���
	// ����ϱ� ���ؼ� Check��� �Լ��� ���� ���Ǿ���� -> ũ�� ���
	// ���ڰ����� �Ǵ��ϱ� ������(���ڴ� ����� �´��� �ƴ���)
	// �ƴϸ� char�� ��ȯ�ϹǷ� �ű⿡ -1�� �Ͽ� true false�� �Ǵ���
	static const bool Value = (bool)(sizeof(Check(p)) - 1);

private:
	TypeCheck() {};
	~TypeCheck() {};
};

class SmartPtr
{
public:
	// ���ø� ������.
	template<typename T>
	friend class KPtr;

private:
	int m_Count;

public:
	SmartPtr();
	virtual ~SmartPtr() = 0;

private:
	void Add_Cnt();
	void Dec_Cnt();
};

template<typename T/*, std::enable_if<std::is_base_of<RefBase, T>::value, T>::type*/>
class KPtr
{
private:
	T* m_Ptr;

public:
	KPtr() : m_Ptr(nullptr) {}

	KPtr(T* _PTR) : m_Ptr(_PTR)
	{
		Ptr_Add();
	}
	KPtr(const KPtr& _PTR) : m_Ptr(_PTR.m_Ptr)
	{
		Ptr_Add();
	}
	KPtr(std::nullptr_t _PTR) : m_Ptr(_PTR)
	{
		// nullptr;
	}
	~KPtr()
	{
		Ptr_Dec();
	}

public:
	bool operator==(std::nullptr_t _PTR) const
	{
		return m_Ptr == _PTR;
	}

	bool operator==(T* _PTR) const
	{
		return m_Ptr == _PTR;
	}

	bool operator==(const KPtr& _PTR) const
	{
		return m_Ptr == _PTR.m_Ptr;
	}

	bool operator!=(std::nullptr_t _PTR) const
	{
		return m_Ptr != _PTR;
	}

	bool operator<(const KPtr& _Other) const
	{
		return m_Ptr < _Other.m_Ptr;
	}
	bool operator>(const KPtr& _Other) const
	{
		return m_Ptr > _Other.m_Ptr;
	}

	// ����ȯ
	operator T*()
	{
		return m_Ptr;
	}

	const T* operator ->() const
	{
		return m_Ptr;
	}


	T* operator->()
	{
		return m_Ptr;
	}

	T& operator *()
	{
		return *m_Ptr;
	}
	

	// static �Ϲ� ���� ĳ����
	// reinterpret ���� ĳ����
	// dynamic ��Ӱ��� ĳ���� - C��Ÿ���� �ƴ� - virtual ��Ӱ����� ����
	// ��Ÿ�� �� ���� -> ���ڿ� ��
	// const const or volatile ���� ��

	template<typename InType>
	operator KPtr<InType>()
	{
		if (nullptr == m_Ptr)
		{
			return nullptr;
		}

		// �Ϲ������� ��ӱ����϶� ����ϴ� ĳ�����Դϴ�.
		// ����� �ƴϸ� nullptr ��ȯ
		InType* P = dynamic_cast<InType*>(m_Ptr);
		if (true == TypeCheck<T, InType>::Value)
		{
			return (InType*)m_Ptr;
		}
		return nullptr;
	}

	KPtr<T>& operator = (T* _PTR)
	{
		if (m_Ptr == _PTR)
		{
			return *this;
		}

		// ���� ���� ������ �ִ� �����ʹ� ����
		Ptr_Dec();
		m_Ptr = _PTR;
		// ���Ӱ� ���� �����ʹ� ����.
		Ptr_Add();
		return *this;
	}

	KPtr<T>& operator = (const KPtr& _PTR)
	{
		if (m_Ptr == _PTR.m_Ptr)
		{
			return *this;
		}

		Ptr_Dec();
		m_Ptr = _PTR.m_Ptr;
		// ���Ӱ� ���� �����ʹ� ����.
		Ptr_Add();
		return *this;
	}


private:
	void Ptr_Add()
	{
		if (nullptr != m_Ptr)
		{
			m_Ptr->Add_Cnt();
		}
	}

	void Ptr_Dec()
	{
		if (nullptr != m_Ptr)
		{
			m_Ptr->Dec_Cnt();
		}
	}
};

template<typename T>
bool operator==(std::nullptr_t _PTR, const KPtr<T>& _KPtr) { return _KPtr == _PTR; }

template<typename T>
bool operator!=(std::nullptr_t _PTR, const KPtr<T>& _KPtr) { return _KPtr != _PTR; }
