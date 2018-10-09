#include "Renderer_Rect3D.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"


Renderer_Rect3D::Renderer_Rect3D()
{
}


Renderer_Rect3D::~Renderer_Rect3D()
{
}

bool Renderer_Rect3D::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);
	if (false == Set_Material(L"IMG_MAT") || false == Set_Mesh(L"RECT3D_MESH"))
	{
		return false;
	}

	return true;
}


void Renderer_Rect3D::Render(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans || false == one()->is_Active())
	{
		return;
	}
	// �θ����� ������� ���Ǿ�� �Ѵ�.
	Indepen_Update();
}