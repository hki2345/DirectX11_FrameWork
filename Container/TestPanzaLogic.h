#pragma once
#include <HLogic.h>
#include <HColCom.h>

class HSpRenderer;
class HSpFrameAnimater;
class HCol2D;
class TestPanzaLogic : public HLogic
{
private:
	size_t m_ImgIndex;
	KPtr<HSpRenderer> ChRender;
	KPtr<HSpRenderer> ChRender2;
	KPtr<HCol2D> m_Col;
	KPtr<HSpFrameAnimater> AniMater;

public:
	bool Init() override;
	void Update() override;
	void DebugRender() override;

public:
	void MyColTest(HColCom* _Left, HColCom* _Right);
	void MyColExitTest(HColCom* _Left, HColCom* _Right);

public:
	TestPanzaLogic();
	~TestPanzaLogic();
};

