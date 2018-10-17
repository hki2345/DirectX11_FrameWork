#pragma once
#include <type_traits>

// 스마트 포인터라는 녀석이 존재한다.
// 스마트 포인터는 간단히 말하면 참조카운트를 통해서 
// 포인터의 메모리를 해제해주는 stl중 하나이다.

// 스마트 포인터에는 2가지 방식이 있는데.
// 첫번째는 스마트포인터클래스가 직접 내부에 레퍼런스를 카운트를 가진 
// 스마트 포인터가 내부에 가지고 있는 경우


// 침습형
// 객체 그자신이 내부에 레퍼런스 카운트를 가지고 있는 방식을 의미한다.

// std::is_base_of<RefBase, T >> , T

// 컴파일러 기반 타입체크를 해주는 구조체이다.

// 다이나믹 캐스트는 런타임때 연산한다. 

// 

template<typename BASE, typename CHILD>
class TYPECHECK
{
private:
	static short Check(BASE*) {};
	static char Check(...) {};

	static CHILD* p;

public:
	// 리터럴 초기화m_ColFi
	static const bool Value = (bool)(sizeof(Check(p)) - 1);

private:
	TYPECHECK() {}
	~TYPECHECK() {}
};

class HRefBase
{
public:
	// 템플릿 프랜드.
	template<typename T>
	friend class HPTR;

private:
	int m_RefCount;

public:
	HRefBase();
	HRefBase(const HRefBase& _Other);
	virtual ~HRefBase() = 0;

private:
	void ADDRef(); // 늘어날때
	void DECRef(); // 줄어들때
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

		// 일반적으로 상속구조일때 사용하는 캐스팅입니다.
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

		// 내가 원래 가지고 있던 포인터는 감소
		DECCHECK();
		PTR = _PTR;
		// 새롭게 받은 포인터는 증가.
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
		// 새롭게 받은 포인터는 증가.
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
