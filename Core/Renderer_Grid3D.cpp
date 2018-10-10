#include "Renderer_Grid3D.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"

#include "DebugManager.h"


Renderer_Grid3D::Renderer_Grid3D()
{
	m_ROption.Deffert_orFoward = 0;
}


Renderer_Grid3D::~Renderer_Grid3D()
{
}


bool Renderer_Grid3D::Init(int _Order /*= 0*/)
{
	m_Size = 10.0f;
	m_Border = 1.0f;
	m_GD.m_Color = KColor::White;

	Renderer::Init(_Order);
	if (false == Set_Material(L"GRID3D_MAT") || false == Set_Mesh(L"RECT3D_MESH"))
	{
		return false;
	}

	if ( nullptr == m_Material->PShader()->Find_ConstBuffer(L"GRIDDATA"))
	{
		m_Material->PShader()->Create_ConstBuf<GRID_DATA>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 2);
	}
	Create_RasterState(L"SOLID_NONE");
	return true;
}


void Renderer_Grid3D::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans || false == one()->is_Active())
	{
		return;
	}

	Set_Data(_Camera);

	// 부모인지 요놈인지 계산되어야 한다.
	Indepen_Update();

	Renderer::Update_Trans(_Camera);
}

void Renderer_Grid3D::Render(KPtr<Camera> _Camera)
{
	m_Material->PShader()->Set_ConstBuffer<GRID_DATA>(L"GRIDDATA", m_GD);
}

void Renderer_Grid3D::Set_Data(KPtr<Camera> _Camera)
{
	float CamY = fabsf(_Camera->one()->World_Pos().y);

	float CheckUper = m_Size;
	float MaxSize = m_Size;

	// 제곱수 변환
	while (MaxSize <= CamY)
	{
		MaxSize *= m_Size;
	}

	MaxSize /= m_Size;

	m_GD.m_GD.x = MaxSize;

	// 0 ~ m_Size 크기 설정
	if (m_Size >= m_GD.m_GD.x)
	{
		MaxSize = m_Size;
		m_GD.m_GD.x = m_Size;
	}

	CheckUper = MaxSize * m_Size;
	m_GD.m_GD.y = CamY * .01f;

	m_GD.m_GD.z = fabsf((CamY - CheckUper) / (CheckUper - MaxSize));


	WLOG(L"Grid Alpha: %f", m_GD.m_GD.z);
	WLOG(L"Camera Y: %f", CamY);
	// 알파는 비율로
	// m_GD.m_GD.z = 1.0f - (CamY / (MaxSize * m_Size));
} 