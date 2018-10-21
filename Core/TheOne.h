#pragma once
#include "Begin_Updater.h"
#include "Mof_KWindow.h"
#include "Mof_State.h"
#include <list>
#include "WriteStream.h"

enum ABS_TYPE
{
	AT_NONE,
	AT_ALLOVER,
	AT_CASEOVER,
};

// 다중상속을 안전하게 쓰려면
// 내부에서 헤더가 필요 없다.
// 상속받지 않은 클래스 여야 한다.
class State;
class Component;
class TransPosition;
class RenderManager;
class HCol2DMgr;
class TheOne final : public Begin_Updater, public Mof_KWindow, public Mof_State
{
public:
	friend State;

	// 어떤 맴버변수가 필요한가?
	// 어떤 함수가 필요한다.
	// 그와 관련있는 클래스나 객체에서 어떻게 처리해 줘야 하는가?

private:
	ABS_TYPE m_AbsType;

	// std::list<std::wstring> m_OverList;

public:
	ABS_TYPE abs_type() {
		return m_AbsType;
	}
	void Set_Abs();

private:
	std::list<KPtr<Component>>::iterator m_ComSI;
	std::list<KPtr<Component>>::iterator m_ComEI;
	std::list<KPtr<Component>> m_ComList;

public:
	std::list<KPtr<Component>> list_component() {
		return m_ComList;
	}

private:
	int m_Order;

public:
	int order() {
		return m_Order;
	}

private:
	void order(int _Order) {
		m_Order = _Order;
	}

private:
	friend TransPosition;
	TransPosition* m_pTrans;

public:
	TransPosition* Trans() 	{		return m_pTrans;	}

private:
	void Trans(TransPosition* _pTrans)	{ m_pTrans = _pTrans; }

private:
	void PrevUpdate() override;
	void Update() override;
	void NextUpdate() override;
	void DebugUpdate() override;
	void DebugRender() override;
	void FinalUpdate();
	void EndUpdate();

	void Release();

public: 
	template<typename Com>
	KPtr<Com> Add_Component()
	{
		if (false == Com::IsMulti(this))
		{
			return nullptr;
		}

		Com* NewCom = new Com();
		NewCom->Set_Type();
		NewCom->one(this);
		NewCom->Window(Window());
		NewCom->state(state());
		NewCom->ComInit();

		if (false == NewCom->Init())
		{
			delete NewCom;
			return nullptr;
		}

		m_ComList.push_back(NewCom);
		return NewCom;
	}

	template<typename Com, typename T1>
	KPtr<Com> Add_Component(T1 _1)
	{
		if (false == Com::IsMulti(this))
		{
			return nullptr;
		}

		Com* NewCom = new Com();
		NewCom->Set_Type();
		NewCom->one(this);
		NewCom->Window(Window());
		NewCom->state(state());
		NewCom->ComInit();

		if (false == NewCom->Init(_1))
		{
			delete NewCom;
			return nullptr;
		}

		m_ComList.push_back(NewCom);
		return NewCom;
	}

	template<typename Com, typename T1, typename T2>
	KPtr<Com> Add_Component(T1 _1, T2 _2)
	{
		if (false == Com::IsMulti(this))
		{
			return nullptr;
		}

		Com* NewCom = new Com();
		NewCom->Set_Type();
		NewCom->one(this);
		NewCom->Window(Window());
		NewCom->state(state());
		NewCom->ComInit();

		if (false == NewCom->Init(_1, _2))
		{
			delete NewCom;
			return nullptr;
		}

		m_ComList.push_back(NewCom);
		return NewCom;
	}

	template<typename Com>
	KPtr<Com> Get_Component()
	{
		std::list<KPtr<Component>>::iterator ComStartIter = m_ComList.begin();
		std::list<KPtr<Component>>::iterator ComEndIter = m_ComList.end();

		for (; ComStartIter != ComEndIter; ++ComStartIter)
		{
			if (true == (*ComStartIter)->IsEqual<Com>())
			{
				return (*ComStartIter);
			}
		}

		return nullptr;
	}

	void Set_Death() override;

private:
	TheOne* m_pParent;
	std::list<KPtr<TheOne>>::iterator ChildStartIter;
	std::list<KPtr<TheOne>>::iterator ChildEndIter;
	std::list<KPtr<TheOne>> m_pChildList; 

	void MoveDetach() { };

public:
	TheOne* Parent() 	{		return m_pParent;	}

private:
	void Detach();

public:
	void AddChild(KPtr<TheOne> _Actor, bool _TransWorld = true);

private:
	void OverPushRender(RenderManager* _pRenMgr);
	void OverPushCol2D(HCol2DMgr* _pCol2DMgr);

public:
	void Save(WriteStream& _Stream);

private:
	TheOne();
	~TheOne();
};

