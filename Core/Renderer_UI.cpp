#include "Renderer_UI.h"
#include "Camera.h"
#include "KWindow.h"


Renderer_UI::Renderer_UI() : m_UISize(KVector::Zero)
{
	m_Bill = true;
}


Renderer_UI::~Renderer_UI()
{
}

bool Renderer_UI::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);
	if (false == Set_Material(L"IMG_MAT") || false == Set_Mesh(L"RECT3D"))
	{
		return false;
	}

	return true;
}


void Renderer_UI::Render(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans || false == one()->is_Active())
	{
		return;
	}

	// 부모인지 요놈인지 계산되어야 한다.
	Indepen_Update();
}


void Renderer_UI::Update_Trans(KPtr<Camera> _Cam)
{
	KASSERT(nullptr == m_Trans);

	Indepen_Update();

	// 크기를 고정크기로 계산
	KMatrix TempMat = KMatrix();
	TempMat.Identity();

	TempMat.vec1.x = m_UISize.x;
	TempMat.vec2.y = m_UISize.y;
	TempMat.vec3.z = m_UISize.z;

	// 고정 크기를 고려한 값으로 재 산출

	if (true == m_Bill)
	{
		m_MD.m_W = (TempMat * m_Trans->Rotate_Matrix() * m_Trans->Position_Matrix()).TransPose_Value();
		m_MD.m_V = _Cam->View().TransPose_Value();
		m_MD.m_P = _Cam->Proj().TransPose_Value();
		m_MD.m_WV = (m_Trans->World_Matrix_Const() * _Cam->View()).TransPose_Referance();
		m_MD.m_WVP = (m_Trans->World_Matrix_Const().Multiply3X3(_Cam->one()->transform()->Rotate_Matrix()) * _Cam->View_Proj()).TransPose_Referance();
	}
	else
	{
		m_MD.m_W = m_Trans->World_Matrix_Const().TransPose_Value();
		m_MD.m_V = _Cam->View().TransPose_Value();
		m_MD.m_P = _Cam->Proj().TransPose_Value();
		m_MD.m_WV = (m_Trans->World_Matrix_Const() * _Cam->View()).TransPose_Referance();
		m_MD.m_WVP = (m_Trans->World_Matrix_Const() * _Cam->View_Proj()).TransPose_Referance();
	}

	Renderer::Update_CB();
}