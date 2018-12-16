#include "KPlane3D_Col.h"



KPlane3D_Col::KPlane3D_Col()
{
	pPlane = CreateColFi<KPlaneCon>(COLTYPE::CT_PLANE3D);
}


KPlane3D_Col::~KPlane3D_Col()
{
}


void KPlane3D_Col::Update()
{
	KVector TT = pPlane->m_ColPoint;
	KLOG(L"%f %f %f", TT.x, TT.y, TT.z);
}

void KPlane3D_Col::Update_Figure()
{
	// KVector RU = _Left.v1;
	// KVector RD = _Left.v2;
	// KVector LU = _Left.v3;
	// KVector LD = _Left.v4;

	// 기본적으로 자신의 크기 - 로 세계에서 돌려본후 충돌했는지 안했는지 판별
	pPlane->m_Mat = KMatrix(
		KVector(m_Trans->pos_world() + (m_Trans->right_local() * m_Trans->scale_local().x + m_Trans->up_local() * m_Trans->scale_local().y) * .5f),
		KVector(m_Trans->pos_world() + (m_Trans->right_local() * m_Trans->scale_local().x + m_Trans->down_local() * m_Trans->scale_local().y) * .5f),
		KVector(m_Trans->pos_world() + (m_Trans->left_local() * m_Trans->scale_local().x + m_Trans->up_local() * m_Trans->scale_local().y) * .5f),
		KVector(m_Trans->pos_world() + (m_Trans->left_local() * m_Trans->scale_local().x + m_Trans->down_local() * m_Trans->scale_local().y) * .5f)
			);

	pPlane->m_Mat.v1.w = .0f;
	pPlane->m_Mat.v2.w = .0f;
	pPlane->m_Mat.v3.w = .0f;
	pPlane->m_Mat.v4.w = .0f;
}


void KPlane3D_Col::DebugRender()
{
	// Update_Figure();
}