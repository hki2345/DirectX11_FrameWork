#pragma once
#include "HUpdaterBase.h"
#include "HActorBase.h"
#include "HWindowBase.h"
#include "HActor.h"

// 크게 기하(트랜스폼, 카메라)
//           3D의 기반 컴포넌트
//      랜더러 (Mesh)
//      콜리전
//      랜더러 

//  리소스 Mesh

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

	// 엔진에서 사용하는 컴포넌트들만 이 함수를 실행할수 있게 하고 싶다.
	virtual void FinalUpdate();
	virtual void EndUpdate();

protected:
	HComponent();
	~HComponent();
};

