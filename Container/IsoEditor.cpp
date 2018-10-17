#include "IsoEditor.h"
#include <HScene.h>
#include <InputMgr.h>
#include <GameDebug.h>


IsoEditor::IsoEditor()
{
}


IsoEditor::~IsoEditor()
{
}

bool IsoEditor::Init() 
{
	IsoRender = GetCom<IsoMapRender>();
	SoundPlayer = AddCom<HSoundPlayer>();

	return true;
}
void IsoEditor::Update() 
{
	HVEC2 MPos = Scene()->Camera()->ScreenToWorld(InputMgr::MousePos());

	if (InputMgr::IsDownStay(L"MouseButton"))
	{
		IsoRender->CreateTile(MPos, 1);
	}

	if (InputMgr::IsDown(L"RMouseButton"))
	{
		SoundPlayer->SoundPlay(L"1-Up.wav");

		HVEC2 MPos = Scene()->Camera()->ScreenToWorld(InputMgr::MousePos());
		std::list<HVEC2> List = IsoRender->WorldPathFind(HVEC2(0.0f, 0.0f), MPos);

		std::list<HVEC2>::iterator StartIter = List.begin();
		std::list<HVEC2>::iterator EndIter = List.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			IsoRender->CreateTile((*StartIter).ix, (*StartIter).iy, 3);
		}

	}
}
void IsoEditor::DebugRender() 
{
	HVEC2 MPos = Scene()->Camera()->ScreenToWorld(InputMgr::MousePos());
	HVEC2 Index = IsoRender->WorldToIndex(MPos);
	wchar_t Arr[256];
	swprintf_s(Arr, L"MWorldPos : %d, %d", Index.ix, Index.iy);
	GameDebug::DrawFont(Arr, { 10.0f, 300.0f }, 20.0f);
}