#include "DebugManager.h"
#include "Core_Class.h"
#include "KWindow.h"
#include "HMesh.h"
#include "HMaterial.h"
#include "HResMgr.h"
#include "HSampler.h"
#include "HFont.h"
#include "HMultiRenderTaget.h"


bool DebugManager::m_bDebug = true;
KColor DebugManager::m_LogColor = KColor::White;
float DebugManager::m_LogSize = 15.0f;
std::wstring	DebugManager::Messege;

std::list<std::wstring>::iterator DebugManager::m_S_Log;
std::list<std::wstring>::iterator DebugManager::m_E_Log;
std::list<std::wstring> DebugManager::m_LogList;


DebugManager::DebugManager()
{
}


DebugManager::~DebugManager()
{
}

void DebugManager::DrawRect(KVector2 _Pos, KVector2 _Size, float _Border/* = 1*/)
{
	DrawRect(KRect( _Pos , _Size ), _Border);
}
// 카메라 행렬이 없을수는 없다.
void DebugManager::DrawRect(const KRect& _Rect, float _Border)
{
	if (nullptr == Core_Class::MainWindow().ThisStateManager.CurScene()->Camera())
	{
		return;
	}

	KVector2 m_Size = Core_Class::MainWindow().ThisStateManager.CurScene()->Camera()->ScreenSize();

	KMatrix m_View;
	KMatrix m_Proj;
	m_View.ViewToLH(KVector4::Zero, KVector4::Forword, KVector4::Up);
	m_Proj.OrthLH(m_Size.x, m_Size.y, 0.1f, 1000.0f);

	KMatrix m1 = Core_Class::MainWindow().ThisStateManager.CurScene()->Camera()->VP();
	KMatrix m2 = m_View * m_Proj;

	KMatrix m_Scale;
	m_Scale.Scale(KVector4(_Rect.m_Size.x, _Rect.m_Size.y, 1.0f));
	KMatrix m_Pos;
	m_Pos.Trans(KVector4(_Rect.m_Pos.x, _Rect.m_Pos.y, 1.1f));

	KPtr<HMesh> Mesh = ResourceManager<HMesh>::Find(L"LINERECTMESH");
	KPtr<HMaterial> Mat = ResourceManager<HMaterial>::Find(L"DEBUGRECTMAT");
	Mat->VTXSH()->SettingCB<KMatrix>(L"TRANS", (m_Scale * m_Pos * m1).RTranspose());

	Mat->Update();
	Mesh->Update();
	Mesh->Render();
}

void DebugManager::DrawFont(wchar_t* _pStr, KVector2 _Pos, float _fSize, KVector4 _COLOR, FW1_TEXT_FLAG _Flag /*= FW1_TEXT_FLAG::FW1_TOP*/) {

	KPtr<HFont> FindFont = ResourceManager<HFont>::Find(L"궁서");

	if (nullptr != FindFont)
	{
		FindFont->DrawFont(_pStr, _Pos, _fSize, _COLOR.color_to_reverse255(), _Flag);

		// FindFont->DrawFont(_pStr, _Pos, _fSize, 0xffffffff, _Flag);
	}
	Core_Class::MainDevice().ResetContext();
}

void DebugManager::DrawLog(wchar_t* _pStr, KVector4 _COLOR /*= KVector4::One*/)
{
	m_LogList.push_back(_pStr);
}

void DebugManager::TagetDebug()
{
	KPtr<HSampler> Smp = ResourceManager<HSampler>::Find(L"DefaultSmp");

	if (nullptr == Smp)
	{
		BBY;
	}
	std::vector<KPtr<HMultiRenderTaget>> Vec = ResourceManager<HMultiRenderTaget>::All_SingleVec();

	KPtr<HMesh> Mesh = ResourceManager<HMesh>::Find(L"RECT");
	KPtr<HMaterial> Mat = ResourceManager<HMaterial>::Find(L"TAGETDEBUGMAT");

	KMatrix m_View;
	KMatrix m_Proj;
	m_View.ViewToLH(KVector4::Zero, KVector4::Forword, KVector4::Up);
	m_Proj.OrthLH(Core_Class::MainWindow().width_f(), Core_Class::MainWindow().height_f(), 0.1f, 1000.0f);

	MatrixContainer tMatData;

	int CountX = 0;
	int CountY = 0;
	int WCount = 5;

	float SizeX = Core_Class::MainWindow().width_f() / WCount;
	float SizeY = Core_Class::MainWindow().height_f() / WCount;

	for (size_t i = 0; i < Vec.size(); i++)
	{
		std::vector<KPtr<HRenderTaget>> TagetVec = Vec[i]->TagetTexList();

		for (size_t j = 0; j < TagetVec.size(); j++)
		{
			KMatrix m_Scale;
			KMatrix m_Pos;

			m_Scale.Iden();
			m_Scale.Scale(KVector4(SizeX, SizeY, 1.0F));
			m_Pos.Iden();
			m_Pos.Trans(
				KVector4( (-Core_Class::MainWindow().width_f() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (Core_Class::MainWindow().height_f() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			KMatrix m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.RTrans();

			Smp->Update(0);
			if (nullptr == TagetVec[j]->TagetTex()->SRV())
			{
				BBY;
			}
			TagetVec[j]->TagetTex()->Update(0);

			Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATDATA", tMatData, SHTYPE::ST_VS);

			Mat->Update();
			Mesh->Update();
			Mesh->Render();

			TagetVec[j]->TagetTex()->Reset(0);

			++CountX;

			if (0 != CountX && 0 == (CountX % 5))
			{
				++CountY;
				CountX = 0;
			}
		}
	}

	CountX = 0;
	CountY += 1;

	std::map<int, KPtr<HCamera>>::iterator m_CamStartIter = Core_Class::MainScene()->RenderMgr.m_CameraMap.begin();
	std::map<int, KPtr<HCamera>>::iterator m_CamEndIter = Core_Class::MainScene()->RenderMgr.m_CameraMap.end();;

	for (; m_CamStartIter != m_CamEndIter; ++m_CamStartIter)
	{
		std::vector<KPtr<HRenderTaget>> TagetVec = m_CamStartIter->second->m_CameraTaget->TagetTexList();

		for (size_t j = 0; j < TagetVec.size(); j++)
		{
			KMatrix m_Scale;
			KMatrix m_Pos;

			m_Scale.Iden();
			m_Scale.Scale(KVector4(SizeX, SizeY, 1.0F));
			m_Pos.Iden();
			m_Pos.Trans(
				KVector4((-Core_Class::MainWindow().width_f() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (Core_Class::MainWindow().height_f() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			KMatrix m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.RTrans();

			Smp->Update(0);
			if (nullptr == TagetVec[j]->TagetTex()->SRV())
			{
				BBY;
			}
			TagetVec[j]->TagetTex()->Update(0);

			Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATDATA", tMatData, SHTYPE::ST_VS);

			Mat->Update();
			Mesh->Update();
			Mesh->Render();

			TagetVec[j]->TagetTex()->Reset(0);

			++CountX;

			if (0 != CountX && 0 == (CountX % 5))
			{
				++CountY;
				CountX = 0;
			}
		}
	}


}

void DebugManager::RenderLog()
{
	m_S_Log = m_LogList.begin();
	m_E_Log = m_LogList.end();

	KVector2 Pos = { 10.0f, 10.0f };

	for (; m_S_Log != m_E_Log; ++m_S_Log)
	{
		// DrawFont((wchar_t*)(*m_S_Log).Msg.c_str(), Pos, m_LogSize, m_LogColor);
		Pos.y += m_LogSize;
	}

	// 일단 dir만 생각하면 되니까.
	// 텍스처 확인은 했다.

	m_LogList.clear();
}