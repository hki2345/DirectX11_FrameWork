#pragma once
#include <type_traits>

// ����Ʈ �����Ͷ�� �༮�� �����Ѵ�.
// ����Ʈ �����ʹ� ������ ���ϸ� ����ī��Ʈ�� ���ؼ� 
// �������� �޸𸮸� �������ִ� stl�� �ϳ��̴�.

// ����Ʈ �����Ϳ��� 2���� ����� �ִµ�.
// ù��°�� ����Ʈ������Ŭ������ ���� ���ο� ���۷����� ī��Ʈ�� ���� 
// ����Ʈ �����Ͱ� ���ο� ������ �ִ� ���


// ħ����
// ��ü ���ڽ��� ���ο� ���۷��� ī��Ʈ�� ������ �ִ� ����� �ǹ��Ѵ�.

// std::is_base_of<RefBase, T >> , T

// �����Ϸ� ��� Ÿ��üũ�� ���ִ� ����ü�̴�.

// ���̳��� ĳ��Ʈ�� ��Ÿ�Ӷ� �����Ѵ�. 

// 

template<typename BASE, typename CHILD>
class TYPECHECK
{
private:
	static short Check(BASE*) {};
	static char Check(...) {};

	static CHILD* p;

public:
	// ���ͷ� �ʱ�ȭm_ColFi
	static const bool Value = (bool)(sizeof(Check(p)) - 1);

private:
	TYPECHECK() {}
	~TYPECHECK() {}
};

class HRefBase
{
public:
	// ���ø� ������.
	template<typename T>
	friend class HPTR;

private:
	int m_RefCount;

public:
	HRefBase();
	HRefBase(const HRefBase& _Other);
	virtual ~HRefBase() = 0;

private:
	void ADDRef(); // �þ��
	void DECRef(); // �پ�鶧
};

template<typename T/*, std::enable_if<std::is_base_of<RefBase, T>::value, T>::type*/>
class HPTR
{
private:
	T* PTR;

public:
	HPTR() : PTR(nullptr) {}

	HPTR(T* _PTR) : PTR(_PTR)
	{
		ADDCHECK();
	}
	HPTR(const HPTR& _PTR) : PTR(_PTR.PTR)
	{
		ADDCHECK();
	}
	HPTR(std::nullptr_t _PTR) : PTR(_PTR)
	{
		// nullptr;
	}
	~HPTR()
	{
		DECCHECK();
	}

public:
	bool operator==(std::nullptr_t _PTR) const
	{
		return PTR == _PTR;
	}

	bool operator==(T* _PTR) const
	{
		return PTR == _PTR;
	}

	bool operator==(const HPTR& _Other) const
	{
		return PTR == _Other.PTR;
	}

	bool operator<(const HPTR& _Other) const
	{
		return PTR < _Other.PTR;
	}

	bool operator>(const HPTR& _Other) const
	{
		return PTR > _Other.PTR;
	}

	bool operator!=(std::nullptr_t _PTR) const
	{
		return PTR != _PTR;
	}

	operator T*() {
		return PTR;
	}

	const T* operator->() const
	{
		return PTR;
	}

	T* operator->()
	{
		return PTR;
	}

	T& operator*()
	{
		return *PTR;
	}

	template<typename InType>
	operator HPTR<InType>()
	{
		if (nullptr == PTR)
		{
			return nullptr;
		}

		// �Ϲ������� ��ӱ����϶� ����ϴ� ĳ�����Դϴ�.
		InType* P = dynamic_cast<InType*>(PTR);
		//std::is_base_of
		if (true == TYPECHECK<T, InType>::Value)
		{
			return (InType*)PTR;
		}
		// 

		return nullptr;
	}

	HPTR<T>& operator = (T* _PTR)
	{
		if (PTR == _PTR)
		{
			return *this;
		}

		// ���� ���� ������ �ִ� �����ʹ� ����
		DECCHECK();
		PTR = _PTR;
		// ���Ӱ� ���� �����ʹ� ����.
		ADDCHECK();
		return *this;
	}

	HPTR<T>& operator = (const HPTR& _PTR)
	{
		if (PTR == _PTR.PTR)
		{
			return *this;
		}

		DECCHECK();
		PTR = _PTR.PTR;
		// ���Ӱ� ���� �����ʹ� ����.
		ADDCHECK();
		return *this;
	}

private:
	void ADDCHECK()
	{
		if (nullptr != PTR)
		{
			PTR->ADDRef();
		}
	}

	void DECCHECK()
	{
		if (nullptr != PTR)
		{
			PTR->DECRef();
		}
	}
};

template<typename T>
bool operator==(std::nullptr_t _PTR, const HPTR<T>& _HPTR) { return _HPTR == _PTR; }

template<typename T>
bool operator!=(std::nullptr_t _PTR, const HPTR<T>& _HPTR) { return _HPTR != _PTR; }
