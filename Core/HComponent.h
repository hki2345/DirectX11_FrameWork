#pragma once
#include "Begin_Updater.h"
#include "HActorBase.h"
#include "Mof_KWindow.h"
#include "HActor.h"

// ũ�� ����(Ʈ������, ī�޶�)
//           3D�� ��� ������Ʈ
//      ������ (Mesh)
//      �ݸ���
//      ������ 

//  ���ҽ� Mesh

class WriteStream;
class ReadStream;
class HComponent : public Begin_Updater, public HActorBase, public Mof_KWindow, public HSceneBase
{
public:
	friend HActor;

private:
	static bool IsMulti(HActor* _pActor);

public:
	template<typename T>
	KPtr<T> GetCom() {
		return Actor()->GetCom<T>();
	}

	template<typename T>
	KPtr<T> AddCom() {
		return Actor()->AddCom<T>();
	}

	template<typename T, typename V1>
	KPtr<T> AddCom(V1 _1) {
		return Actor()->AddCom<T>(_1);
	}

public:
	virtual void Save(WriteStream& _Stream);
	virtual void Load(ReadStream& _Stream);

public:
	virtual void ComInit();

	// �������� ����ϴ� ������Ʈ�鸸 �� �Լ��� �����Ҽ� �ְ� �ϰ� �ʹ�.
	virtual void FinalUpdate();
	virtual void EndUpdate();

protected:
	HComponent();
	~HComponent();
};

