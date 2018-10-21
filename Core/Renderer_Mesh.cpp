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
void Renderer_Mesh::Render(KPtr<Camera> _Camera)
{
}

bool Renderer_Mesh::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);

	SetRSState(L"SBACK");
	return true;
}

