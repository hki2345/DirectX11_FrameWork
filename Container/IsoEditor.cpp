#include "IsoEditor.h"
#include <State.h>
#include <InputManager.h>
#include <DebugManager.h>


IsoEditor::IsoEditor()
{
}


IsoEditor::~IsoEditor()
{
}

bool IsoEditor::Init() 
{
	IsoRender = Get_Component<IsoMapRender>();
	m_SndPlayer = Add_Component<SoundPlayer>();

	return true;
}
void IsoEditor::Update() 
{
	KVector2 MPos = state()->Camera()->ScreenTo_World(InputManager::MousePos());

	if (InputManager::Press(L"MouseButton"))
	{
		IsoRender->CreateTile(MPos, 1);
	}

	if (InputManager::Down(L"RMouseButton"))
	{
		m_SndPlayer->Play(L"1-Up.wav");

		KVector2 MPos = state()->Camera()->ScreenTo_World(InputManager::MousePos());
		std::list<KVector2> List = IsoRender->WorldPathFind(KVector2(0.0f, 0.0f), MPos);

		std::list<KVector2>::iterator StartIter = List.begin();
		std::list<KVector2>::iterator EndIter = List.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			IsoRender->CreateTile((*StartIter).ix, (*StartIter).iy, 3);
		}

	}
}
void IsoEditor::DebugRender() 
{
	KVector2 MPos = state()->Camera()->ScreenTo_World(InputManager::MousePos());
	KVector2 Index = IsoRender->WorldToIndex(MPos);
	wchar_t Arr[256];
	swprintf_s(Arr, L"MWorldPos : %d, %d", Index.ix, Index.iy);
	DebugManager::Draw_Font(Arr, { 10.0f, 300.0f }, 20.0f);
}