#include "TestSceneBuilder.h"

#include <TheOne.h>
#include <KWindow.h>
#include <Camera.h>
#include <TransPosition.h>
#include <HSpRenderer.h>
#include <KCollider2D_DE.h>

#include "TestPanzaLogic.h"
#include "TestSceneUpdater.h"



TestSceneBuilder::TestSceneBuilder()
{
}


TestSceneBuilder::~TestSceneBuilder()
{
}

void TestSceneBuilder::Build_State()
{
	// 첫목표는 판자 하나를 띄운다.

	// 결국 어떤 기하정보를 
	// 만들어내기 위해서는 각 버텍스들에 곱해질 필수적인 행렬이 필요하다.

	// 오브젝트 그자체의 행렬
	// (내가 지금 여기에 있다.)
	// 뷰행렬
	// (그걸 내가 이렇게 보고 있다.)
	// 투영행렬
	// (3D 그걸 어떤 시각으로 비출것이냐)

	// 뷰포트행렬

	// 카메라
	// 오브젝트하나
	// 매쉬하나

	state()->This_Col2DManager.Link(0, 10);

	// 카메라가 우선이다.
	// 트랜스
	KPtr<TheOne> Cam = state()->Create_One();
	KPtr<Camera> CamCom = Cam->Add_Component<Camera>();
	CamCom->Insert_LayerData(0, 1, 2, 3, 4);

	KPtr<TheOne> TestPanza1 = state()->Create_One();
	TestPanza1->Trans()->scale_local({ 106 * 2, 71 * 2, 1});
	TestPanza1->Trans()->pos_local({ 100, 0, 10 });

	KPtr<KCollider2D_DE> Test = TestPanza1->Add_Component<KCollider2D_DE>();
	 KPtr<TestPanzaLogic> TestPan = TestPanza1->Add_Component<TestPanzaLogic>();

	Test->EnterFunc<TestPanzaLogic>(TestPan, &TestPanzaLogic::MyColTest);
	Test->ExitFunc<TestPanzaLogic>(TestPan, &TestPanzaLogic::MyColExitTest);

/*
	KPtr<TheOne> Child = state()->Create_One();
	Child->AddCom<HSpRenderer>(L"Rock.png");
	Child->Trans()->scale_local({ 50, 50, 1 });
	Child->Trans()->LPos({ 300, 0, 5 });

	TestPanza1->AddChild(Child);

	KPtr<TheOne> Monster = state()->Create_One();
	Monster->AddCom<HSpRenderer>(L"Test.png");
	Test = Monster->AddCom<HCol2D>(10);
	Test->Mode(COLTYPE::CT_RECT2D);

	Monster->Trans()->scale_local({ 50, 50, 1 });
	Monster->Trans()->LPos({ -100, 0, 5 });*/

	//KPtr<TheOne> TestPanza2 = state()->Create_One();
	//TestPanza2->AddCom<TestPanzaLogic>();
	//TestPanza2->Trans()->scale_local({ 500, 300, 1 });
	//TestPanza2->Trans()->LPos({ -100, 0, 20 });

	// KPtr<TheOne> Camera = state()->Create_One();
}