#include "TestSceneBuilder.h"

#include <HActor.h>
#include <HWindow.h>
#include <HCamera.h>
#include <HTrans.h>
#include <HSpRenderer.h>
#include <HCol2D.h>

#include "TestPanzaLogic.h"
#include "TestSceneUpdater.h"



TestSceneBuilder::TestSceneBuilder()
{
}


TestSceneBuilder::~TestSceneBuilder()
{
}

void TestSceneBuilder::SceneBuild()
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

	Scene()->Col2DMgr.Link(0, 10);

	// 카메라가 우선이다.
	// 트랜스
	HPTR<HActor> Camera = Scene()->CreateActor();
	HPTR<HCamera> CamCom = Camera->AddCom<HCamera>();
	CamCom->PushRenderLayer(0, 1, 2, 3, 4);

	HPTR<HActor> TestPanza1 = Scene()->CreateActor();
	TestPanza1->Trans()->LScale({ 106 * 2, 71 * 2, 1});
	TestPanza1->Trans()->LPos({ 100, 0, 10 });

	HPTR<HCol2D> Test = TestPanza1->AddCom<HCol2D>();
	Test->Mode(COLTYPE::CT_RECT2D);
	 HPTR<TestPanzaLogic> TestPan = TestPanza1->AddCom<TestPanzaLogic>();

	Test->EnterFunc<TestPanzaLogic>(TestPan, &TestPanzaLogic::MyColTest);
	Test->ExitFunc<TestPanzaLogic>(TestPan, &TestPanzaLogic::MyColExitTest);

/*
	HPTR<HActor> Child = Scene()->CreateActor();
	Child->AddCom<HSpRenderer>(L"Rock.png");
	Child->Trans()->LScale({ 50, 50, 1 });
	Child->Trans()->LPos({ 300, 0, 5 });

	TestPanza1->AddChild(Child);

	HPTR<HActor> Monster = Scene()->CreateActor();
	Monster->AddCom<HSpRenderer>(L"Test.png");
	Test = Monster->AddCom<HCol2D>(10);
	Test->Mode(COLTYPE::CT_RECT2D);

	Monster->Trans()->LScale({ 50, 50, 1 });
	Monster->Trans()->LPos({ -100, 0, 5 });*/

	//HPTR<HActor> TestPanza2 = Scene()->CreateActor();
	//TestPanza2->AddCom<TestPanzaLogic>();
	//TestPanza2->Trans()->LScale({ 500, 300, 1 });
	//TestPanza2->Trans()->LPos({ -100, 0, 20 });

	// HPTR<HActor> Camera = Scene()->CreateActor();
}