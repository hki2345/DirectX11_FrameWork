#pragma once
#include "Begin_Updater.h"
#include "HActorBase.h"
#include "Mof_KWindow.h"
#include "HActor.h"

// 크게 기하(트랜스폼, 카메라)
//           3D의 기반 컴포넌트
//      랜더러 (Mesh)
//      콜리전
//      랜더러 

//  리소스 Mesh

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

	// 엔진에서 사용하는 컴포넌트들만 이 함수를 실행할수 있게 하고 싶다.
	virtual void FinalUpdate();
	virtual void EndUpdate();

protected:
	HComponent();
	~HComponent();
};

