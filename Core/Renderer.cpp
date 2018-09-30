#include "Renderer.h"
#include "KMacro.h"
#include "ResourceManager.h"
#include "State.h"
#include "KDevice.h"
#include "KWindow.h"
#include "Core_Class.h"

Renderer::Renderer() :m_RasterState(nullptr)
{
}


Renderer::~Renderer()
{
}

void Renderer::Create_RasterState(const wchar_t* _Name)
{
	m_RasterState = window()->device().Find_RasterMode(_Name);
	if (nullptr == m_RasterState)
	{
		KASSERT(true);
		return;
	}
}

bool Renderer::Init(int _Order /*= 0*/)
{
	m_Order = _Order;
	state()->this_RenderManager.Insert_Renderer(this);
	return true;
}

bool Renderer::Set_Mesh(const wchar_t* _Name)
{
	m_Mesh = ResourceManager<Mesh>::Find(_Name);
	KASSERT(nullptr == m_Mesh); 

	if (nullptr == m_Mesh)
	{
		return false;
	}

	return true;
}
bool Renderer::Set_Mesh()
{
	m_Mesh = ResourceManager<Mesh>::Find(name());
	KASSERT(nullptr == m_Mesh);

	if (nullptr == m_Mesh)
	{
		return false;
	}

	return true;
}


bool Renderer::Set_Material(const wchar_t* _Name)
{
	m_Material = ResourceManager<Material>::Find(_Name);
	KASSERT(nullptr == m_Material);

	if (nullptr == m_Material)
	{
		return false;
	}

	return true;
}

void Renderer::RenderUpdate()
{
	if (nullptr != m_RasterState)
	{
		m_RasterState->Update();
	}
}

void Renderer::RenderFinalUpdate()
{
	window()->device().ResetRasterState();
}


void Renderer::Update_Trans(KPtr<Camera> _Cam)
{
	m_MD.m_W = m_Trans->World_Matrix_Const().TransPose_Value();
	m_MD.m_V = _Cam->View().TransPose_Value();
	m_MD.m_P = _Cam->Proj().TransPose_Value();
	m_MD.m_WVP = (m_Trans->World_Matrix_Const() * _Cam->View_Proj()).TransPose_Referance();

	Update_CB();
}
void Renderer::Update_CB()
{
	Core_Class::MainDevice().Set_DeviceCB<DATA_3D>(L"DATA3D", m_MD, SHADER_TYPE::ST_VS);
	Core_Class::MainDevice().Set_DeviceCB<DATA_3D>(L"DATA3D", m_MD, SHADER_TYPE::ST_PS);
}
void Renderer::Update_MeshMat()
{
	m_Material->Update();
	m_Mesh->Update();
	m_Mesh->Render();
}