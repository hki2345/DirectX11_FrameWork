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
	// ��Ʈ�� ���� ����� ����
	return true;
}
void Renderer_Effect::Update()
{

}

// �ش� ȿ�� ������ ������ ���� ������ ���� �κ��̴�.
void Renderer_Effect::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	// ������� ��Ʈ�� ���۸� ��� �ֽ�ȭ �ϴ� ���� ���� ����
	// �� ������� ���۴� �츮�� �ε����� ���ؽ��� �Ϥ����� ������ �־��ִ� ���� �ƴ�
	// �ڵ����� �ֿ��� �ȴ�.
	Core_Class::Context()->DrawAuto();
}
