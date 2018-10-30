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

	// Ʈ���� �� ���� �ݸ��� ��� ���� ����� �ڿ� ī�޶� ������ ����
	// ��ǥ�� ó��
	// ���������� ���� -> �ھ����
	virtual void FinalUpdate();
	virtual void EndUpdate();

};

