#include "Renderer_Effect.h"
#include "Core_Class.h"


Renderer_Effect::Renderer_Effect()
{
}


Renderer_Effect::~Renderer_Effect()
{
}

bool Renderer_Effect::Init(const KUINT& _DotCnt, const int& _Order/* = 0*/)
{
	// 스트림 버퍼 만드는 구간
	return true;
}
void Renderer_Effect::Update()
{

}

// 해당 효과 랜더는 일일이 직접 구현이 들어가는 부분이다.
void Renderer_Effect::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	// 만들어진 스트림 버퍼를 어떻게 최신화 하는 지에 대한 구간
	// 단 만들어진 버퍼는 우리가 인덱스와 버텍스를 일ㅇ일이 지정해 넣어주는 것이 아닌
	// 자동으로 넣우제 된다.
	Core_Class::Context()->DrawAuto();
}
