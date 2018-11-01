#include "TestPanzaLogic.h"
#include <HSpRenderer.h>
#include <InputManager.h>
#include <HSpFrameAnimater.h>
#include <KCollider2D_DE.h>
#include <DebugManager.h>

TestPanzaLogic::TestPanzaLogic() : m_ImgIndex(0)
{
}


TestPanzaLogic::~TestPanzaLogic()
{
}

bool TestPanzaLogic::Init() 
{
	ChRender = Add_Component<HSpRenderer>(L"Test.png");
	ChRender2 = Add_Component<HSpRenderer>(L"BW.png");
	// ChRender2->Color(KColor(1.0f, 0.0f, 0.0f, 1.0f));

	AniMater = Add_Component<HSpFrameAnimater>();
	AniMater->SettingSpRender(ChRender2);
	AniMater->CreateAniL(L"Test1", L"BW.png", 0, 7, false);
	AniMater->CreateAniL(L"Test2", L"BW.png", 19, 32, false);
	AniMater->ChangeAni(L"Test1");

	ChRender->SelfMode();
	ChRender->SubPivot({ 0, 100.0f, 0 });
	ChRender->SubScale({ 100.0f, 100.0f, 0 });

	m_Col = Get_Component<KCollider2D_DE>();
	return true;
}

void TestPanzaLogic::Update()  
{
	if (InputManager::Press(L"Q"))
	{
		m_Trans->Rotating_Deg(KVector4::Up * 360.0f * TimeManager::DeltaTime());
	}

	if (InputManager::Press(L"E"))
	{
		m_Trans->Rotating_Deg(KVector4::Down * 360.0f * TimeManager::DeltaTime());
	}

	if (InputManager::Press(L"XS"))
	{
		m_Trans->Moving(KVector4::Left * 100.0f * TimeManager::DeltaTime());
	}

	if (InputManager::Down(L"LEFT"))
	{

		AniMater->ChangeAni(L"Test2");

		//--m_ImgIndex;
		//ChRender->ImageIndex(m_ImgIndex);
	} 

	if (InputManager::Down(L"RIGHT"))
	{
		//++m_ImgIndex;
		//ChRender->ImageIndex(m_ImgIndex);
	}

	if (InputManager::Press(L"UP"))
	{
		m_Trans->Moving(m_Trans->left_local() *100.0f * TimeManager::DeltaTime());
	}

	if (InputManager::Press(L"DOWN"))
	{
		m_Trans->Moving(m_Trans->right_local() * 100.0f * TimeManager::DeltaTime());
	}

	if (InputManager::Down(L"Death"))
	{
		one()->Set_Death();
	}
}

void TestPanzaLogic::MyColTest(KCollision* _Left, KCollision* _Right) {
	int a = 0;
}

void TestPanzaLogic::MyColExitTest(KCollision* _Left, KCollision* _Right) {
	int a = 0;
}

void TestPanzaLogic::DebugRender() 
{
	// DebugManager::Draw_Font(L"гогого", {100.0f, 100.0f }, 50.0f, 0xff000000);
}