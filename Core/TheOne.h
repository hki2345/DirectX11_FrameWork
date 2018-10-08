#pragma once
#include "Begin_Updater.h"
#include "Mother_KWindow.h"
#include "Mother_State.h"
#include "DirectXHeader.h"
#include "WriteStream.h"


#include <list>


enum OBJ_ABSTYPE
{
	NONE_ABS,
	ALL_ABS,
	PART_ABS,
};

// ���� -> final -> ���̻� ����� ������ �ʴ´�.
class State;
class Component;
class TransPosition; // Ʈ���� ������ Ư������ - ���־��� ���� ������� �����̱⿡
class RenderManager;
class Collider2DManager;
class TheOne final: public Begin_Updater, public Mother_KWindow, public Mother_State
{
public:
	friend State;

	OBJ_ABSTYPE m_AbsType;

public:
	OBJ_ABSTYPE& abs_type() { return m_AbsType; }
	void all_abs();

private:
	std::list<KPtr<Component>> m_Componentlist;
	std::list<KPtr<Component>>::iterator m_Com_StartIter;
	std::list<KPtr<Component>>::iterator m_Com_EndIter;

	// ������ �� �� ��ȯ
public:
	std::list<KPtr<Component>> component_list()
	{
		return m_Componentlist;
	}

private:
	int m_Order;

public:
	int order() const { return m_Order; }
	void order(const int& _Value) { m_Order = _Value; }

private:
	friend TransPosition;
	// HPTR�� �ϸ� ��ȯ������ �Ͼ ���ɼ��� ����
	TransPosition* one_Trans;

public:
	TransPosition* transform() { return one_Trans; }

private:
	// �ٸ� �������� set�� �� �� ����
	void transform(TransPosition* _Value) { one_Trans = _Value; }

private:
	void PrevUpdate() override ;
	void Update() override ;
	void NextUpdate() override ;
	void FinalUpdate() ;
	void UIRender() override;
	void DebugUpdate() override;
	void DebugRender() override;

	// Componet�� End_Update�� �����Ѵ�.
	void End_Update();
	void Release();

public:
	template<typename Comp>
	KPtr<Comp> Add_Component()
	{
		if (false == Comp::Is_Multi(this))
		{
			return nullptr;
		}

		Comp* NewCom = new Comp();
		NewCom->set_type();
		NewCom->one(this);
		NewCom->window(window());
		NewCom->state(state());
		NewCom->Init_Component();

		if (false == NewCom->Init())
		{
			delete NewCom;
			return nullptr;
		}

		m_Componentlist.push_back(NewCom);
		return NewCom;
	}

	template<typename Comp, typename T1>
	// ������ ����
	KPtr<Comp> Add_Component(T1 _1)
	{
		if (false == Comp::Is_Multi(this))
		{
			return nullptr;
		}

		Comp* NewCom = new Comp();
		NewCom->set_type();
		NewCom->one(this);
		NewCom->window(window());
		NewCom->state(state());
		NewCom->Init_Component();

		if (false == NewCom->Init(_1))
		{
			delete NewCom;
			return nullptr;
		}

		m_Componentlist.push_back(NewCom);
		return NewCom;
	}


	template<typename Comp, typename T1, typename T2>
	// �̼Ҹ�Ʈ�� ����
	KPtr<Comp> Add_Component(T1 _1, T2 _2)
	{
		if (false == Comp::Is_Multi(this))
		{
			return nullptr;
		}

		Comp* NewCom = new Comp();
		NewCom->set_type();
		NewCom->one(this);
		NewCom->window(window());
		NewCom->state(state());
		NewCom->Init_Component();

		if (false == NewCom->Init(_1, _2))
		{
			delete NewCom;
			return nullptr;
		}

		m_Componentlist.push_back(NewCom);
		return NewCom;
	}

	template<typename Comp, typename T1, typename T2, typename T3>
	// �̼Ҹ�Ʈ�� ����
	KPtr<Comp> Add_Component(T1 _1, T2 _2, T3 _3)
	{
		if (false == Comp::Is_Multi(this))
		{
			return nullptr;
		}

		Comp* NewCom = new Comp();
		NewCom->set_type();
		NewCom->one(this);
		NewCom->window(window());
		NewCom->state(state());
		NewCom->Init_Component();

		if (false == NewCom->Init(_1, _2, _3))
		{
			delete NewCom;
			return nullptr;
		}

		m_Componentlist.push_back(NewCom);
		return NewCom;
	}


	template<typename Com>
	KPtr<Com> Get_Component()
	{
		std::list<KPtr<Component>>::iterator Com_StartIter = m_Componentlist.begin();
		std::list<KPtr<Component>>::iterator Com_EndIter = m_Componentlist.end();

		for (; Com_StartIter != Com_EndIter; ++Com_StartIter)
		{
			if (true == (*Com_StartIter)->is_equal<Com>() && false == (*Com_StartIter)->is_Death())
			{
				return (*Com_StartIter);
			}
		}

		return nullptr;
	}

	void set_Death() override;

private:
	TheOne* m_Parent;
	std::list<KPtr<TheOne>> m_ChildList;
	std::list<KPtr<TheOne>>::iterator m_Child_StartIter;
	std::list<KPtr<TheOne>>::iterator m_Child_EndIter;

	// ���� �ǹپ� ��Ȯ�� �˰���
	void MoveDetach() {  }

public:
	TheOne* parent() 
	{
		return m_Parent;
	}

	KVector4 World_Scale();
	KVector4 World_Pos();
	KVector4 World_Rotate();
	KVector4 World_Forward();
	KVector4 World_Up();
	KVector4 World_Right();
	KVector4 World_Back();
	KVector4 World_Down();
	KVector4 World_Left();

	KVector4 Local_Scale();
	KVector4 Local_Pos();
	KVector4 Local_Rotate();
	KVector4 Local_Forward();
	KVector4 Local_Up();
	KVector4 Local_Right();
	KVector4 Local_Back();
	KVector4 Local_Down();
	KVector4 Local_Left();

	bool BWorld() const;

	void World_Scale(const KVector4& _Value);
	void World_Pos(const KVector4& _Value);
	void World_Rotate(const KVector4& _Value);

	void World_Scale(const float& _X, const float& _Y, const float& _Z);
	void World_Pos(const float& _X, const float& _Y, const float& _Z);
	void World_Rotate(const float& _X, const float& _Y, const float& _Z);

	void World_Scale(const KVector2& _Vec, const float& _Z);
	void World_Pos(const KVector2& _Vec, const float& _Z);
	void World_Rotate(const KVector2& _Vec, const float& _Z);



	void Local_Scale(const KVector4& _Value);
	void Local_Pos(const KVector4& _Value);
	void Local_Rotate(const KVector4& _Value);

	void Local_Scale(const float& _X, const float& _Y, const float& _Z);
	void Local_Pos(const float& _X, const float& _Y, const float& _Z);
	void Local_Rotate(const float& _X, const float& _Y, const float& _Z);

	void Local_Scale(const KVector2& _Vec, const float& _Z);
	void Local_Pos(const KVector2& _Vec, const float& _Z);
	void Local_Rotate(const KVector2& _Vec, const float& _Z);

	void detach();
	void insert_Child(KPtr<TheOne> _Child, const bool& _World = true);

	void Set_ImageSize();

	void Set_Image_RatioW(const float& _Value);
	void Set_Image_RatioH(const float& _Value);

private:
	void insert_AbsRender(RenderManager* _Render);
	void insert_AbsCol2D(Collider2DManager* _Col2D);

public:
	void BillBoard();
	void save(WriteStream& _Stream);

public:
	TheOne();
	~TheOne();
};

