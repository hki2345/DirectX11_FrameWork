#pragma once
#include "Begin_Updater.h"
#include "Mof_One.h"
#include "Mof_KWindow.h"
#include "TheOne.h"



class WriteStream;
class ReadStream;
class Component : public Begin_Updater, public Mof_One, public Mof_KWindow, public Mof_State
{
protected:
	Component();
	~Component();


public:
	friend TheOne;

private:
	static bool IsMulti(TheOne* _pActor);

public:
	template<typename T>
	KPtr<T> Get_Component() {
		return one()->Get_Component<T>();
	}

	template<typename T>
	KPtr<T> Add_Component() {
		return one()->Add_Component<T>();
	}

	template<typename T, typename V1>
	KPtr<T> Add_Component(V1 _1) {
		return one()->Add_Component<T>(_1);
	}

public:
	virtual void ComInit();

	// 트렌스 및 랜더 콜리젼 모든 것을 계산한 뒤에 카메라에 보여질 최종
	// 좌표들 처리
	// 엔진에서만 사용될 -> 코어에서만
	virtual void FinalUpdate();
	virtual void EndUpdate();

};

