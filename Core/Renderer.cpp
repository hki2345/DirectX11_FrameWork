#include "Renderer.h"
#include "KMacro.h"
#include "ResourceManager.h"
#include "State.h"
#include "KDevice.h"
#include "KWindow.h"
#include "KMaterial.h"
#include "Core_Class.h"

Renderer::Renderer() : m_RsState(nullptr)
{
	if (false == Set_Material(L"NONEMAT"))
	{
		BBY;
	}
}


Renderer::~Renderer()
{
}

void Renderer::Set_RSState(const wchar_t* _Name) {
	m_RsState = kwindow()->Device().Find_RS(_Name);

	if (nullptr == m_RsState)
	{
		KASSERT(true);
		return;
	}
}

bool Renderer::Init(int _Order)
{
	m_Order = _Order;
	state()->RenderMgr.Insert_Renderer(this);
	return true;
}

bool Renderer::Set_Mesh(KPtr<KMesh> _Mesh, const int& _Index)
{
	KASSERT(nullptr == _Mesh);

	if (m_MeshVec.size() <= _Index)
	{
		m_MeshVec.resize(_Index + 1);
	}

	m_MeshVec[_Index] = _Mesh;
	KASSERT(nullptr == m_MeshVec[_Index]);

	if (nullptr == m_MeshVec[_Index])
	{
		return false;
	}
	return true;
}

bool Renderer::Set_Mesh(const wchar_t* _Res, const int& _Index) 
{
	if (m_MeshVec.size() <= _Index)
	{
		m_MeshVec.resize(_Index + 1);
	}

	m_MeshVec[_Index] = ResourceManager<KMesh>::Find(_Res);
	KASSERT(nullptr == m_MeshVec[_Index]);

	if (nullptr == m_MeshVec[_Index])
	{
		return false;
	}
	return true;
}

bool Renderer::Set_Material(const wchar_t* _Res, const int& _Index)
{
	if (m_MtlVec.size() <= _Index)
	{
		m_MtlVec.resize(_Index + 1);
	}

	m_MtlVec[_Index] = ResourceManager<KMaterial>::Find(_Res);
	KASSERT(nullptr == m_MtlVec[_Index]);

	if (nullptr == m_MtlVec[_Index])
	{
		return false;
	}

	return true;
}

KPtr<KMaterial> Renderer::material(const int& _Index) 
{
	if (m_MtlVec[_Index]->m_bOrigin)
	{
		m_MtlVec[_Index] = m_MtlVec[_Index]->Clone();
	}

	return m_MtlVec[_Index];
}

void Renderer::RenderUpdate()
{
	if (nullptr != m_RsState)
	{
		m_RsState->Update();
	}
}

void Renderer::RenderFinalUpdate() 
{
	kwindow()->Device().Reset_RS();
}

void Renderer::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate(); 

	m_MD.m_W = m_Trans->worldmat_const().RVTranspose();
	m_MD.m_V = _Camera->View().RVTranspose();
	m_MD.m_P = _Camera->Proj().RVTranspose();
	m_MD.m_WV = (m_Trans->worldmat_const() * _Camera->View()).RTranspose();
	m_MD.m_WVP = (m_Trans->worldmat_const() * _Camera->ViewProj()).RTranspose();
}

void Renderer::Update_TransCB() 
{
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_PS);
}




void Renderer::Update_MtlCB(const KUINT _Index /*= 0*/)
{
	if (nullptr != m_MtlVec[_Index])
	{
		ROpt.TexCnt = m_MtlVec[_Index]->Set_TexData(ROpt.TexArr);
	}


	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<RenderOption>(L"RENDEROPTION", ROpt, SHTYPE::ST_PS);
}

void Renderer::Update_TexSmp(const KUINT _Index/* = 0*/)
{
	if (nullptr != m_MtlVec[_Index])
	{
		m_MtlVec[_Index]->Update_Tex();
		m_MtlVec[_Index]->Update_Sam();
	}
	else
	{
		BBY;
	}
}

void Renderer::Update_Material(const KUINT _Index /*= 0*/)
{
	if (nullptr != m_MtlVec[_Index])
	{
		m_MtlVec[_Index]->Update();
	}
	else
	{
		BBY;
	}
}

void Renderer::Update_Mesh(const KUINT _Index /*= 0*/)
{
	if (nullptr != m_MeshVec[_Index])
	{
		m_MeshVec[_Index]->Update();
		m_MeshVec[_Index]->Render();
	}
	else
	{
		BBY;
	}
}




// ·»´õµ¥ÀÌÅÍ¸¦ ½¦ÀÌ´õ·Î º¸³»±â À§ÇØ ¼¼ÆÃÀÌ ¸ÕÀú µÇ¾î¾ß ÇÑ´Ù. - ­w°¡ÇØ¾ß ÇÑ´Ù.
// ±× ÀÛ¾÷
void Renderer::Insert_RenderData(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Sub, const KUINT _Mat)
{
	m_RDVec.push_back({ _Mesh, _Vtx, _Sub, _Mat });
}

// ÇØ´ç ¸Þ½¬ µû·Î ÃÖ½ÅÈ­
void Renderer::Update_SelectMesh(const KUINT& _Mesh, const KUINT& _Vtx, const KUINT& _Idx)
{
	if (nullptr != m_MeshVec[_Mesh])
	{
		m_MeshVec[_Mesh]->Update_Pick(_Vtx, _Idx);
	}
	else
	{
		BBY;
	}
}