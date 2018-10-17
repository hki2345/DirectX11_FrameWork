#include "TestPanzaLogic.h"
#include <HSpRenderer.h>
#include <InputMgr.h>
#include <HSpFrameAnimater.h>
#include <HCol2D.h>
#include <GameDebug.h>

TestPanzaLogic::TestPanzaLogic() : m_ImgIndex(0)
{
}


TestPanzaLogic::~TestPanzaLogic()
{
}

bool TestPanzaLogic::Init() 
{
	ChRender = AddCom<HSpRenderer>(L"Test.png");
	ChRender2 = AddCom<HSpRenderer>(L"BW.png");
	// ChRender2->Color(HCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	AniMater = AddCom<HSpFrameAnimater>();
	AniMater->SettingSpRender(ChRender2);
	AniMater->CreateAniL(L"Test1", L"BW.png", 0, 7, false);
	AniMater->CreateAniL(L"Test2", L"BW.png", 19, 32, false);
	AniMater->ChangeAni(L"Test1");

	ChRender->SelfMode();
	ChRender->SubPivot({ 0, 100.0f, 0 });
	ChRender->SubScale({ 100.0f, 100.0f, 0 });

	m_Col = GetCom<HCol2D>();
	return true;
}

void TestPanzaLogic::Update()  
{
	if (InputMgr::IsDownStay(L"Q"))
	{
		m_Trans->LAccRotDeg(HVEC::MRZ * 360.0f * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDownStay(L"E"))
	{
		m_Trans->LAccRotDeg(HVEC::RZ * 360.0f * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDownStay(L"XS"))
	{
		m_Trans->LMove(HVEC::LEFT * 100.0f * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDown(L"LEFT"))
	{

		AniMater->ChangeAni(L"Test2");

		//--m_ImgIndex;
		//ChRender->ImageIndex(m_ImgIndex);
	} 

	if (InputMgr::IsDown(L"RIGHT"))
	{
		//++m_ImgIndex;
		//ChRender->ImageIndex(m_ImgIndex);
	}

	if (InputMgr::IsDownStay(L"UP"))
	{
		m_Trans->LMove(m_Trans->LLeft() *100.0f * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDownStay(L"DOWN"))
	{
		m_Trans->LMove(m_Trans->LRight() * 100.0f * TimeMgr::DeltaTime());
	}

	if (InputMgr::IsDown(L"Death"))
	{
		Actor()->Death();
	}
}

void TestPanzaLogic::MyColTest(HColCom* _Left, HColCom* _Right) {
	int a = 0;
}

void TestPanzaLogic::MyColExitTest(HColCom* _Left, HColCom* _Right) {
	int a = 0;
}

void TestPanzaLogic::DebugRender() 
{
	// GameDebug::DrawFont(L"гогого", {100.0f, 100.0f }, 50.0f, 0xff000000);
}