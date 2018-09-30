#include "Renderer_Mesh.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"
#include "ResourceManager.h"




Renderer_Mesh::Renderer_Mesh()
{
}


Renderer_Mesh::~Renderer_Mesh()
{
}


bool Renderer_Mesh::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);

	if (false == Set_Material(L"COLOR_MAT") || false == Set_Mesh(L"CUBE3D_MESH"))
	{
		return false;
	}


	Create_RasterState(L"SBACK");

	return true;
}


void Renderer_Mesh::Render(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans || false == one()->is_Active())
	{
		return;
	}
	// 부모인지 요놈인지 계산되어야 한다.
	Indepen_Update();

	if (nullptr != m_Image)
	{
		m_Image->sampler()->Update();
		m_Image->texture()->Update();
	}
}


void Renderer_Mesh::image(const wchar_t* _Name)
{
	m_Image = ResourceManager<KImage>::Find(_Name);
	if (nullptr == m_Image)
	{
		KASSERT(true);
	}
}