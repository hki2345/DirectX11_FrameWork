#pragma once
#include "Begin_Updater.h"
#include "TheOne.h"

#include "Mother_State.h"
#include "Mother_TheOne.h"
#include "Mother_KWindow.h"

#include "TimeManager.h"

class WriteStream;
class ReadStream;
class Component :
	public Begin_Updater, public Mother_TheOne, public Mother_KWindow, public Mother_State
{
public:
	friend TheOne;

public:
	static bool Is_Multi(TheOne* _pTheOne);
	
public:
	virtual void Init_Component();

	// 트렌스 및 랜더 콜리젼 모든 것을 계산한 뒤에 카메라에 보여질 최종
	// 좌표들 처리
	// 엔진에서만 사용될 -> 코어에서만
	virtual void FinalUpdate();
	virtual void End_Update();


public:
	template<typename T>
	KPtr<T> Get_Component()
	{
		return one()->Get_Component<T>();
	}

	template<typename T>
	KPtr<T> Add_Component()
	{
		return one()->Add_Component<T>();
	}


	template<typename T, typename V1>
	KPtr<T> Add_Component(V1 _1)
	{
		return one()->Add_Component<T>(_1);
	}

public:
	virtual void save(WriteStream& _Stream);
	virtual void load(ReadStream& _Stream);

public:
	Component();
	~Component();
};

