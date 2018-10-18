#pragma once
#include "HUpdaterBase.h"
#include "HWindowBase.h"
#include "HSceneBase.h"
#include <list>
#include "BWStream.h"

enum OVERTYPE
{
	OT_NONE,
	OT_ALLOVER,
	OT_CASEOVER,
};

// 다중상속을 안전하게 쓰려면
// 내부에서 헤더가 필요 없다.
// 상속받지 않은 클래스 여야 한다.
class HScene;
class HComponent;
class HTrans;
class HRenderMgr;
class HCol2DMgr;
class HActor final : public HUpdaterBase, public HWindowBase, public HSceneBase
{
public:
	friend HScene;

	// 어떤 맴버변수가 필요한가?
	// 어떤 함수가 필요한다.
	// 그와 관련있는 클래스나 객체에서 어떻게 처리해 줘야 하는가?

private:
	OVERTYPE m_OverType;

	// std::list<std::wstring> m_OverList;

public:
	OVERTYPE OverType() {
		return m_OverType;
	}
	void Over();

private:
	std::list<KPtr<HComponent>>::iterator m_ComStartIter;
	std::list<KPtr<HComponent>>::iterator m_ComEndIter;
	std::list<KPtr<HComponent>> m_ComList;

public:
	std::list<KPtr<HComponent>> AllComList() {
		return m_ComList;
	}

private:
	int m_Order;

public:
	int Order() {
		return m_Order;
	}

private:
	void Order(int _Order) {
		m_Order = _Order;
	}

private:
	friend HTrans;
	HTrans* m_pTrans;

public:
	HTrans* Trans() 	{		return m_pTrans;	}

private:
	void Trans(HTrans* _pTrans)	{ m_pTrans = _pTrans; }

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
	KPtr<Com> AddCom()
	{
		if (false == Com::IsMulti(this))
		{
			return nullptr;
		}

		Com* NewCom = new Com();
		NewCom->Set_Type();
		NewCom->Actor(this);
		NewCom->Window(Window());
		NewCom->Scene(Scene());
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
	KPtr<Com> AddCom(T1 _1)
	{
		if (false == Com::IsMulti(this))
		{
			return nullptr;
		}

		Com* NewCom = new Com();
		NewCom->Set_Type();
		NewCom->Actor(this);
		NewCom->Window(Window());
		NewCom->Scene(Scene());
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
	KPtr<Com> AddCom(T1 _1, T2 _2)
	{
		if (false == Com::IsMulti(this))
		{
			return nullptr;
		}

		Com* NewCom = new Com();
		NewCom->Set_Type();
		NewCom->Actor(this);
		NewCom->Window(Window());
		NewCom->Scene(Scene());
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
	KPtr<Com> GetCom()
	{
		std::list<KPtr<HComponent>>::iterator ComStartIter = m_ComList.begin();
		std::list<KPtr<HComponent>>::iterator ComEndIter = m_ComList.end();

		for (; ComStartIter != ComEndIter; ++ComStartIter)
		{
			if (true == (*ComStartIter)->IsEqual<Com>())
			{
				return (*ComStartIter);
			}
		}

		return nullptr;
	}

	void Death() override;

private:
	HActor* m_pParent;
	std::list<KPtr<HActor>>::iterator ChildStartIter;
	std::list<KPtr<HActor>>::iterator ChildEndIter;
	std::list<KPtr<HActor>> m_pChildList; 

	void MoveDetach() { };

public:
	HActor* Parent() 	{		return m_pParent;	}

private:
	void Detach();

public:
	void AddChild(KPtr<HActor> _Actor, bool _TransWorld = true);

private:
	void OverPushRender(HRenderMgr* _pRenMgr);
	void OverPushCol2D(HCol2DMgr* _pCol2DMgr);

public:
	void Save(BWStream& _Stream);

private:
	HActor();
	~HActor();
};

