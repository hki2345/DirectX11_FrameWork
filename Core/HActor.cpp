#include "HActor.h"
#include "HComponent.h"
#include "KMacro.h"
#include "HScene.h"
#include "Core_Class.h"
#include "HRenderMgr.h"
#include "HCol2DMgr.h"


HActor::HActor()
{
}

HActor::~HActor()
{
}

void HActor::Over() 
{
	m_OverType = OT_ALLOVER;

	if (true == Window()->SceneMgr.OverFind(this))
	{
		return;
	}

	Window()->SceneMgr.m_OverActorList.push_back(this);

	std::unordered_map<std::wstring, KPtr<HScene>>::iterator StartMapIter = Window()->SceneMgr.m_SceneMap.begin();
	std::unordered_map<std::wstring, KPtr<HScene>>::iterator EndMapIter = Window()->SceneMgr.m_SceneMap.end();

	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		if (StartMapIter->second == Scene())
		{
			continue;
		}

		StartMapIter->second->OverPushActor(this);
	}
}

void HActor::PrevUpdate() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->Is_Active())
		{
			continue;
		}

		(*m_ComStartIter)->PrevUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->PrevUpdate();
	}

}

void HActor::Update() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->Is_Active())
		{
			continue;
		}

		(*m_ComStartIter)->Update();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->Update();
	}
}

void HActor::NextUpdate() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->Is_Active())
		{
			continue;
		}

		(*m_ComStartIter)->NextUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->NextUpdate();
	}
}

void HActor::DebugUpdate() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->Is_Active())
		{
			continue;
		}

		(*m_ComStartIter)->DebugUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->DebugUpdate();
	}
}

void HActor::FinalUpdate() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->Is_Active())
		{
			continue;
		}

		(*m_ComStartIter)->FinalUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->FinalUpdate();
	}
}

void HActor::DebugRender() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->Is_Active())
		{
			continue;
		}

		(*m_ComStartIter)->DebugRender();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->DebugRender();
	}
}

void HActor::EndUpdate() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->Is_Active())
		{
			continue;
		}

		(*m_ComStartIter)->EndUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->EndUpdate();
	}
}

void HActor::Release() 
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; )
	{
		if (true == (*m_ComStartIter)->Is_Death())
		{
			m_ComStartIter = m_ComList.erase(m_ComStartIter);
		}
		else {
			++m_ComStartIter;
		}
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->Release();
	}
}

void HActor::Set_Death() 
{
	Begin_Updater::Set_Death();

	std::list<KPtr<HComponent>>::iterator ComStartIter = m_ComList.begin();
	std::list<KPtr<HComponent>>::iterator ComEndIter = m_ComList.end();

	for (; ComStartIter != ComEndIter; ++ComStartIter)
	{
		(*ComStartIter)->Set_Death();
	}
}

void HActor::AddChild(KPtr<HActor> _Actor, bool _TransWorld /*= true*/)
{
	if (OT_NONE != _Actor->OverType())
	{
		return;
	}

	if (false == Scene()->EraseActor(_Actor))
	{
		return;
	}

	_Actor->m_pParent = this;
	m_pChildList.push_back(_Actor);

	if (true == _TransWorld && nullptr != Trans() && nullptr != _Actor->Trans())
	{
		// 100, 100, 100
		// 100, 100, 100
		// 크기
		HVEC CScale = _Actor->Trans()->LScale();
		HVEC Scale = Trans()->LScale();

		// HVEC R = CScale / Scale;

		_Actor->Trans()->LScale(CScale / Scale);

		// 100, 100 // 자식
		// 50, 50 // 부모
		HVEC CPos = _Actor->Trans()->LPos();
		HVEC Pos = Trans()->LPos();
		_Actor->Trans()->LPos((CPos - Pos) / Trans()->LScale());

		// 90 
		// 90
		// 회전
		HVEC CRot = _Actor->Trans()->LRot();
		HVEC Rot = Trans()->LRot();
		_Actor->Trans()->LRot(CRot - Rot);
	}
	else if (false == _TransWorld && nullptr != Trans() && nullptr != _Actor->Trans())
	{
		_Actor->Trans()->Reset();
	}

} 



void HActor::Detach() 
{
	std::list<KPtr<HActor>>::iterator StartIter = m_pParent->m_pChildList.begin();
	std::list<KPtr<HActor>>::iterator EndIter = m_pParent->m_pChildList.end();

	bool ChildCheck = false;
	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter) == this)
		{
			m_pParent->m_pChildList.erase(StartIter);
			ChildCheck = true;
			break;
		}
	}

	if (false == ChildCheck)
	{
		KASSERT(false == ChildCheck);
		return;
	}
	// m_pParent->Scene()->add
}

#include "HRenderer.h"

void HActor::OverPushRender(HRenderMgr* _pRenMgr)
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->IsParent<HRenderer>())
		{
			continue;
		}
		_pRenMgr->PushOverRenderer((*m_ComStartIter));
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->OverPushRender(_pRenMgr);
	}
}

void HActor::OverPushCol2D(HCol2DMgr* _pCol2DMgr)
{
	m_ComStartIter = m_ComList.begin();
	m_ComEndIter = m_ComList.end();

	for (; m_ComStartIter != m_ComEndIter; ++m_ComStartIter)
	{
		if (false == (*m_ComStartIter)->IsParent<HCol2DBase>())
		{
			continue;
		}
		_pCol2DMgr->Push2DCol((*m_ComStartIter));
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->OverPushCol2D(_pCol2DMgr);
	}
}

void HActor::Save(BWStream& _Stream) 
{
	UINT Size = Order();
	_Stream.Write(&Size, sizeof(UINT));

	Size = NameSize() + 1;
	_Stream.Write(&Size, sizeof(UINT));
	_Stream.WriteT(Name(), sizeof(wchar_t) * Size);

	Size = (UINT)m_ComList.size();
	_Stream.Write(&Size, sizeof(UINT));

	std::list<KPtr<HComponent>>::iterator ComStartIter = m_ComList.begin();
	std::list<KPtr<HComponent>>::iterator ComEndIter = m_ComList.end();

	for (; ComStartIter != ComEndIter; ++ComStartIter)
	{
		(*ComStartIter)->Save(_Stream);
	}

	Size = (UINT)m_pChildList.size();
	_Stream.Write(&Size, sizeof(UINT));

	std::list<KPtr<HActor>>::iterator tempChildStartIter = m_pChildList.begin();
	std::list<KPtr<HActor>>::iterator tempChildEndIter = m_pChildList.end();

	for (; tempChildStartIter != tempChildEndIter; ++tempChildStartIter)
	{
		(*ChildStartIter)->Save(_Stream);
	}
}