#include "H3DGRID.h"
#include "KMacro.h"
#include "KWindow.h"
#include "HTrans.h"
#include "DebugManager.h"


H3DGRID::H3DGRID() : m_BlockSizeInter(10.0f)
{
	ROpt.IsDefferdAndForward = 0;
}


H3DGRID::~H3DGRID()
{
}

void H3DGRID::CalData(KPtr<HCamera> _Camera) 
{

	float Y = fabsf(_Camera->Trans()->WPos().y);
	float CheckUper = m_BlockSizeInter;
	float Check = m_BlockSizeInter;

	while (Check <= Y)	{
		Check *= m_BlockSizeInter;
	}

	Check *= 0.1f;

	m_GridData.x = Check;

	if (m_BlockSizeInter >= m_GridData.x)
	{
		Check = m_BlockSizeInter;
		m_GridData.x = m_BlockSizeInter;
	}

	CheckUper = Check * m_BlockSizeInter;

	m_GridData.y = Y * 0.01f;

	m_GridData.z = fabsf((Y - CheckUper) / (CheckUper - Check));

	wchar_t Arr[256];

	swprintf_s(Arr, L"Alpha : %f", m_GridData.z);
	DebugManager::DrawLog(Arr);
}

void H3DGRID::TransUpdate(KPtr<HCamera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	CalData(_Camera);

	SubTransUpdate();

	m_MatData.m_W = m_Trans->CWMat();

	m_MatData.m_W.v4.x = Scene()->Camera()->Trans()->WPos().x;
	m_MatData.m_W.v4.y = Scene()->Camera()->Trans()->WPos().y;

	m_MatData.m_V = _Camera->CV();
	m_MatData.m_P = _Camera->CP();
	m_MatData.m_WVP = (CSWMat() * _Camera->VP()).RTranspose();

	HRenderer::TransCBUpdate();
}

void H3DGRID::Render(KPtr<HCamera> _Camera)
{
	m_Mat->PIXSH()->SettingCB<KVector4>(L"GRIDDATA", m_GridData);
}

bool H3DGRID::Init(int _Order /*= 0*/)
{
	HRenderer::Init(_Order);

	if (false == SetMat(L"GRID3DMAT"))
	{
		return false;
	}
	if (false == SetMesh(L"RECT"))
	{
		return false;
	}

	//if (nullptr == Window()->Device().FindCB(L"GRIDDATA"))
	//{
	//	Window()->Device().CreateCB<KVector4>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 0);
	//}

	SetRSState(L"SNONE");

	return true;
}


void H3DGRID::DebugRender() 
{


}