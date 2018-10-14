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

	Create_RasterState(L"SBACK");
	Set_Mesh(L"CUBE");

	return true;
}


void Renderer_Mesh::Render(KPtr<Camera> _Camera)
{
}