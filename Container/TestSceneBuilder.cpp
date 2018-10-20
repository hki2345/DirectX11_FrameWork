#include "TestSceneBuilder.h"

#include <HActor.h>
#include <KWindow.h>
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
	// ù��ǥ�� ���� �ϳ��� ����.

	// �ᱹ � ���������� 
	// ������ ���ؼ��� �� ���ؽ��鿡 ������ �ʼ����� ����� �ʿ��ϴ�.

	// ������Ʈ ����ü�� ���
	// (���� ���� ���⿡ �ִ�.)
	// �����
	// (�װ� ���� �̷��� ���� �ִ�.)
	// �������
	// (3D �װ� � �ð����� ������̳�)

	// ����Ʈ���

	// ī�޶�
	// ������Ʈ�ϳ�
	// �Ž��ϳ�

	Scene()->Col2DMgr.Link(0, 10);

	// ī�޶� �켱�̴�.
	// Ʈ����
	KPtr<HActor> Camera = Scene()->CreateActor();
	KPtr<HCamera> CamCom = Camera->AddCom<HCamera>();
	CamCom->PushRenderLayer(0, 1, 2, 3, 4);

	KPtr<HActor> TestPanza1 = Scene()->CreateActor();
	TestPanza1->Trans()->LScale({ 106 * 2, 71 * 2, 1});
	TestPanza1->Trans()->LPos({ 100, 0, 10 });

	KPtr<HCol2D> Test = TestPanza1->AddCom<HCol2D>();
	Test->Mode(COLTYPE::CT_RECT2D);
	 KPtr<TestPanzaLogic> TestPan = TestPanza1->AddCom<TestPanzaLogic>();

	Test->EnterFunc<TestPanzaLogic>(TestPan, &TestPanzaLogic::MyColTest);
	Test->ExitFunc<TestPanzaLogic>(TestPan, &TestPanzaLogic::MyColExitTest);

/*
	KPtr<HActor> Child = Scene()->CreateActor();
	Child->AddCom<HSpRenderer>(L"Rock.png");
	Child->Trans()->LScale({ 50, 50, 1 });
	Child->Trans()->LPos({ 300, 0, 5 });

	TestPanza1->AddChild(Child);

	KPtr<HActor> Monster = Scene()->CreateActor();
	Monster->AddCom<HSpRenderer>(L"Test.png");
	Test = Monster->AddCom<HCol2D>(10);
	Test->Mode(COLTYPE::CT_RECT2D);

	Monster->Trans()->LScale({ 50, 50, 1 });
	Monster->Trans()->LPos({ -100, 0, 5 });*/

	//KPtr<HActor> TestPanza2 = Scene()->CreateActor();
	//TestPanza2->AddCom<TestPanzaLogic>();
	//TestPanza2->Trans()->LScale({ 500, 300, 1 });
	//TestPanza2->Trans()->LPos({ -100, 0, 20 });

	// KPtr<HActor> Camera = Scene()->CreateActor();
}