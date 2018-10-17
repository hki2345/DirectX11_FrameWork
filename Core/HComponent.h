#pragma once
#include "HUpdaterBase.h"
#include "HActorBase.h"
#include "HWindowBase.h"
#include "HActor.h"

// ũ�� ����(Ʈ������, ī�޶�)
//           3D�� ��� ������Ʈ
//      ������ (Mesh)
//      �ݸ���
//      ������ 

//  ���ҽ� Mesh

class BWStream;
class BRStream;
class HComponent : public HUpdaterBase, public HActorBase, public HWindowBase, public HSceneBase
{
public:
	friend HActor;

private:
	static bool IsMulti(HActor* _pActor);

public:
	template<typename T>
	HPTR<T> GetCom() {
		return Actor()->GetCom<T>();
	}

	template<typename T>
	HPTR<T> AddCom() {
		return Actor()->AddCom<T>();
	}

	template<typename T, typename V1>
	HPTR<T> AddCom(V1 _1) {
		return Actor()->AddCom<T>(_1);
	}

public:
	virtual void Save(BWStream& _Stream);
	virtual void Load(BRStream& _Stream);

public:
	virtual void ComInit();

	// �������� ����ϴ� ������Ʈ�鸸 �� �Լ��� �����Ҽ� �ְ� �ϰ� �ʹ�.
	virtual void FinalUpdate();
	virtual void EndUpdate();

protected:
	HComponent();
	~HComponent();
};

