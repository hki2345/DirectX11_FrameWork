#pragma once
#include <set>
#include <unordered_map>
#include <list>
#include "DirectXHeader.h"
#include "Collider2D.h"

class State;
class Collider2DManager
{
public:
	friend State;

private:
	std::set<__int64>::iterator S_Col_Iter;
	std::set<__int64>::iterator E_Col_Iter;
	std::set<__int64> m_Chain;

	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator L_Iter;
	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator R_Iter;
	std::unordered_map<int, std::list<KPtr<Collision>>> m_Collider2D_Map;

	std::list<KPtr<Collision>>::iterator LS_Iter;
	std::list<KPtr<Collision>>::iterator LE_Iter;
	std::list<KPtr<Collision>>::iterator RS_Iter;
	std::list<KPtr<Collision>>::iterator RE_Iter;

public:
	void chain(const int& _Value);
	void chain(const int& _Left, const int& _Right);
	void insert_2D(KPtr<Collision> _Col);

private:
	void progress();
	void release();

public:
	void insert_AbsCollider2D(KPtr<Collider2D> _Col);
	KPtr<Collision> All_Collision_Check(Collision* _Other);
	KPtr<Collision> Collision_Check(const int& _Order, Collision* _Other);

public:
	Collider2DManager();
	~Collider2DManager();
};

