#include "GameDebug.h"
#include "HVAR.h"
#include "HWindow.h"
#include "HMesh.h"
#include "HMaterial.h"
#include "HResMgr.h"
#include "HSampler.h"
#include "HFont.h"
#include "HMultiRenderTaget.h"


bool GameDebug::m_IsDebug = true;
std::list<GameDebug::LogMsg>::iterator GameDebug::m_LogStart;
std::list<GameDebug::LogMsg>::iterator GameDebug::m_LogEnd;
std::list<GameDebug::LogMsg> GameDebug::m_Log;
float GameDebug::LogSize = 20.0f;

GameDebug::GameDebug()
{
}


GameDebug::~GameDebug()
{
}

void GameDebug::DrawRect(HVEC2 _Pos, HVEC2 _Size, float _Border/* = 1*/) 
{
	DrawRect(HRECT( _Pos , _Size ), _Border);
}
// ī�޶� ����� �������� ����.
void GameDebug::DrawRect(const HRECT& _Rect, float _Border) 
{
	if (nullptr == HVAR::MainWindow().SceneMgr.CurScene()->Camera())
	{
		return;
	}

	HVEC2 m_Size = HVAR::MainWindow().SceneMgr.CurScene()->Camera()->ScreenSize();

	HMAT m_View;
	HMAT m_Proj;
	m_View.ViewToLH(HVEC::ZERO, HVEC::FORWORD, HVEC::UP);
	m_Proj.OrthLH(m_Size.x, m_Size.y, 0.1f, 1000.0f);

	HMAT m1 = HVAR::MainWindow().SceneMgr.CurScene()->Camera()->VP();
	HMAT m2 = m_View * m_Proj;

	HMAT m_Scale;
	m_Scale.Scale(HVEC(_Rect.m_Size.x, _Rect.m_Size.y, 1.0f));
	HMAT m_Pos;
	m_Pos.Trans(HVEC(_Rect.m_Pos.x, _Rect.m_Pos.y, 1.1f));

	HPTR<HMesh> Mesh = HResMgr<HMesh>::Find(L"LINERECTMESH");
	HPTR<HMaterial> Mat = HResMgr<HMaterial>::Find(L"DEBUGRECTMAT");
	Mat->VTXSH()->SettingCB<HMAT>(L"TRANS", (m_Scale * m_Pos * m1).RTranspose());

	Mat->Update();
	Mesh->Update();
	Mesh->Render();
}

void GameDebug::DrawFont(wchar_t* _pStr, HVEC2 _Pos, float _fSize, HVEC _COLOR, FW1_TEXT_FLAG _Flag /*= FW1_TEXT_FLAG::FW1_TOP*/) {

	HPTR<HFont> FindFont = HResMgr<HFont>::Find(L"�ü�");

	if (nullptr != FindFont)
	{
		FindFont->DrawFont(_pStr, _Pos, _fSize, _COLOR.Color255Convert(), _Flag);

		// FindFont->DrawFont(_pStr, _Pos, _fSize, 0xffffffff, _Flag);
	}
	HVAR::MainDevice().ResetContext();
}

void GameDebug::DrawLog(wchar_t* _pStr, HVEC _COLOR /*= HVEC::ONE*/)
{
	m_Log.push_back({ _pStr , _COLOR });
}

void GameDebug::TagetDebug() 
{
	HPTR<HSampler> Smp = HResMgr<HSampler>::Find(L"DefaultSmp");

	if (nullptr == Smp)
	{
		BOOM;
	}
	std::vector<HPTR<HMultiRenderTaget>> Vec = HResMgr<HMultiRenderTaget>::GetAllSingleRes();

	HPTR<HMesh> Mesh = HResMgr<HMesh>::Find(L"RECT");
	HPTR<HMaterial> Mat = HResMgr<HMaterial>::Find(L"TAGETDEBUGMAT");

	HMAT m_View;
	HMAT m_Proj;
	m_View.ViewToLH(HVEC::ZERO, HVEC::FORWORD, HVEC::UP);
	m_Proj.OrthLH(HVAR::MainWindow().FWidth(), HVAR::MainWindow().FHeight(), 0.1f, 1000.0f);

	MATDATA tMatData;

	int CountX = 0;
	int CountY = 0;
	int WCount = 5;

	float SizeX = HVAR::MainWindow().FWidth() / WCount;
	float SizeY = HVAR::MainWindow().FHeight() / WCount;

	for (size_t i = 0; i < Vec.size(); i++)
	{
		std::vector<HPTR<HRenderTaget>> TagetVec = Vec[i]->TagetTexList();

		for (size_t j = 0; j < TagetVec.size(); j++)
		{
			HMAT m_Scale;
			HMAT m_Pos;

			m_Scale.Iden();
			m_Scale.Scale(HVEC(SizeX, SizeY, 1.0F));
			m_Pos.Iden();
			m_Pos.Trans(
				HVEC( (-HVAR::MainWindow().FWidth() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (HVAR::MainWindow().FHeight() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			HMAT m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.RTrans();

			Smp->Update(0);
			if (nullptr == TagetVec[j]->TagetTex()->SRV())
			{
				BOOM;
			}
			TagetVec[j]->TagetTex()->Update(0);

			HVAR::MainDevice().SettingCB<MATDATA>(L"MATDATA", tMatData, SHTYPE::ST_VS);

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

	std::map<int, HPTR<HCamera>>::iterator m_CamStartIter = HVAR::MainScene()->RenderMgr.m_CameraMap.begin();
	std::map<int, HPTR<HCamera>>::iterator m_CamEndIter = HVAR::MainScene()->RenderMgr.m_CameraMap.end();;

	for (; m_CamStartIter != m_CamEndIter; ++m_CamStartIter)
	{
		std::vector<HPTR<HRenderTaget>> TagetVec = m_CamStartIter->second->m_CameraTaget->TagetTexList();

		for (size_t j = 0; j < TagetVec.size(); j++)
		{
			HMAT m_Scale;
			HMAT m_Pos;

			m_Scale.Iden();
			m_Scale.Scale(HVEC(SizeX, SizeY, 1.0F));
			m_Pos.Iden();
			m_Pos.Trans(
				HVEC((-HVAR::MainWindow().FWidth() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (HVAR::MainWindow().FHeight() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			HMAT m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.RTrans();

			Smp->Update(0);
			if (nullptr == TagetVec[j]->TagetTex()->SRV())
			{
				BOOM;
			}
			TagetVec[j]->TagetTex()->Update(0);

			HVAR::MainDevice().SettingCB<MATDATA>(L"MATDATA", tMatData, SHTYPE::ST_VS);

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

void GameDebug::RenderLog() 
{
	m_LogStart = m_Log.begin();
	m_LogEnd = m_Log.end();

	HVEC2 Pos = { 10.0f, 10.0f };

	for (; m_LogStart != m_LogEnd; ++m_LogStart)
	{
		DrawFont((wchar_t*)(*m_LogStart).Msg.c_str(), Pos, LogSize,(*m_LogStart).m_COLOR);
		Pos.y += LogSize;
	}

	// �ϴ� dir�� �����ϸ� �Ǵϱ�.
	// �ؽ�ó Ȯ���� �ߴ�.

	m_Log.clear();
}