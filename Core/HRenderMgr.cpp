#include "HRenderMgr.h"
#include "HRenderer.h"
#include "HCamera.h"
#include "HMACRO.h"
#include "HLight.h"
#include "HResMgr.h"
#include "HSampler.h"
#include "GameDebug.h"
#include "HMultiRenderTaget.h"
#include "HVAR.h"

HRenderMgr::HRenderMgr()
{
}


HRenderMgr::~HRenderMgr()
{
}

// 순서로 처리하는 식으로 바뀔수도 2d에서만
bool HRenderMgr::ZOrderSort(HPTR<HRenderer> _Left, HPTR<HRenderer> _Right)
{
	if (_Left->WSubPivot().z < _Right->WSubPivot().z)
	{
		return true;
	}

	return false;
}

void HRenderMgr::ResetSR() 
{
	HPTR<HSampler> Smp = HResMgr<HSampler>::Find(L"DefaultSmp");

	if (nullptr == Smp)
	{
		BOOM;
	}

	Smp->Update(0);
	Smp->Update(1);
	Smp->Update(2);
	Smp->Update(3);
	Smp->Update(4);
	Smp->Update(5);
	Smp->Update(6);
	Smp->Update(7);

}


void HRenderMgr::Render() 
{
	HVAR::MainDevice().ClearTaget();
	HVAR::MainDevice().OMSet(); // 0

	ResetSR();

	m_CamStartIter = m_CameraMap.begin();
	m_CamEndIter = m_CameraMap.end();

	for (; m_CamStartIter != m_CamEndIter; ++m_CamStartIter)
	{
		bool Check = false;
		for (size_t i = 0; i < m_CamStartIter->second->m_RenderGroup.size(); ++i)
		{
			m_GFindIter = m_RenderMap.find(m_CamStartIter->second->m_RenderGroup[i]);

			if (m_GFindIter == m_RenderMap.end())
			{
				continue;
			}
			HVAR::MainDevice().SetDsMode(L"BASIC");
			// 디퍼드타겟 그린다.
			Render_Defferd(m_CamStartIter->second, m_GFindIter, i);
			// 라이트 연산을 한다.

			HVAR::MainDevice().SetDsMode(L"LIGHTDEPTH");
			// 디퍼드는 끈다는 개념을 하던가 해야한다.
			Render_Defferd_Light(m_CamStartIter->second, (int)i);

			m_CamStartIter->second->m_CameraTaget->Clear();
			m_CamStartIter->second->m_CameraTaget->OMSet();
			m_CamStartIter->second->LightMerge();
			// 타겟 머지

			HVAR::MainDevice().SetDsMode(L"BASIC");
			// 다시 켜야되.
			Render_Forward(m_CamStartIter->second, m_GFindIter, i);

			// 최종 나온 결과물을 

			// 카메라가 하나 랜더타겟을 가지고 있을 것이다.
			// (*m_CamStartIter)->


		} // for (; m_GroupStartIter != m_GroupEndIter; ++m_GroupStartIter)
	} // for (; m_CamStartIter != m_CamEndIter; ++m_CamStartIter)

	HVAR::MainDevice().OMSet();
	ScreenMerge();
	

	if (GameDebug::IsDebug())
	{
		HVAR::MainDevice().SetDsMode(L"ALWAYS");
		HVAR::MainScene()->DebugRender();
		GameDebug::TagetDebug();
		GameDebug::RenderLog();
	}

	
	HVAR::MainDevice().Present();
	// 카메라의 랜더타겟들을 합쳐서
	// 백버퍼 텍스처에 그릴 것이다.

}

void HRenderMgr::PushCamera(HCamera* _Camera)
{
	TASSERT(nullptr == _Camera);
	m_CameraMap.insert(std::map<int, HPTR<HCamera>>::value_type(_Camera->Order(), _Camera));
}

void HRenderMgr::PushRenderer(HRenderer* _Renderer) 
{
	TASSERT(nullptr == _Renderer);

	m_GFindIter = m_RenderMap.find(_Renderer->Order());

	if (m_GFindIter == m_RenderMap.end())
	{
		m_RenderMap.insert(std::unordered_map<int, std::list<HPTR<HRenderer>>>::value_type(_Renderer->Order(), std::list<HPTR<HRenderer>>()));
		m_GFindIter = m_RenderMap.find(_Renderer->Order());
	}

	m_GFindIter->second.push_back(_Renderer);
}


void HRenderMgr::PushOverRenderer(HPTR<HRenderer> _Renderer)
{
	TASSERT(nullptr == _Renderer);

	m_GFindIter = m_RenderMap.find(_Renderer->Order());

	if (m_GFindIter == m_RenderMap.end())
	{
		m_RenderMap.insert(std::unordered_map<int, std::list<HPTR<HRenderer>>>::value_type(_Renderer->Order(), std::list<HPTR<HRenderer>>()));
		m_GFindIter = m_RenderMap.find(_Renderer->Order());
	}

	m_GFindIter->second.push_back(_Renderer);
}

void HRenderMgr::Release() {
	m_RAStartIter = m_RenderMap.begin();
	m_RAEndIter = m_RenderMap.end();

	for (; m_RAStartIter != m_RAEndIter; ++m_RAStartIter)
	{
		m_RStartIter = m_RAStartIter->second.begin();
		m_REndIter = m_RAStartIter->second.end();

		for (; m_RStartIter != m_REndIter; )
		{
			if (true == (*m_RStartIter)->IsDeath())
			{
				m_RStartIter = m_RAStartIter->second.erase(m_RStartIter);
				// m_REndIter = m_RAStartIter->second.end();
			}
			else {
				++m_RStartIter;
			}
		}
	}
}


//////////////////////////////////////////// Light
void HRenderMgr::PushLight(HLight* _Light) 
{
	m_LightSet.insert(_Light);
}

void HRenderMgr::LightCheck(HPTR<HCamera> _Camera, int _Group)
{
	m_LightStartIter = m_LightSet.begin();
	m_LightEndIter = m_LightSet.end();

	int Count = 0;
	HLight::LightCBDATA Data;
	memset(&Data, 0, sizeof(HLight::LightCBDATA));

	for (; m_LightStartIter != m_LightEndIter; ++m_LightStartIter)
	{
		if (true == (*m_LightStartIter)->IsLight(_Group))
		{
			HPTR<HLight> Ptr = (*m_LightStartIter);
			Ptr->CalLightData(_Camera);
			Data.ArrLight[Count] = (*m_LightStartIter)->Data;

			++Count;
			if (10 <= Count)
			{
				break;
			}
		}
	}

	Data.LightCount = Count;

	HVAR::MainDevice().SettingCB<HLight::LightCBDATA>(L"LIGHTDATA", Data, SHTYPE::ST_VS);
	HVAR::MainDevice().SettingCB<HLight::LightCBDATA>(L"LIGHTDATA", Data, SHTYPE::ST_PS);

	// HDevice::SettingCB()

	// 이건 어느 쉐이더에도 속하지 않는 아주. 글로벌한 녀석이어야 한다.
	// 상수버퍼에 세팅해줘야 하는데 전역 상수버퍼가 없다.

	return;
}

void HRenderMgr::Render_Defferd(HPTR<HCamera> _Camera, std::map<int, std::list<HPTR<HRenderer>>>::iterator _Iter, size_t _Index)
{
	// 디퍼드용 메테리얼로 
	HPTR<HMultiRenderTaget> DEFFERDTAGET = HResMgr<HMultiRenderTaget>::Find(L"DEFFERD");
	DEFFERDTAGET->Clear();
	DEFFERDTAGET->OMSet();

	HPTR<HMaterial> DEFFERD3DMAT = HResMgr<HMaterial>::Find(L"DEFFERD3DMAT");
	TASSERT(nullptr == DEFFERD3DMAT);
	if (nullptr == DEFFERD3DMAT)
	{
		BOOM;
	}

	m_RStartIter = m_GFindIter->second.begin();
	m_REndIter = m_GFindIter->second.end();
	for (; m_RStartIter != m_REndIter; m_RStartIter++)
	{
		if (1 == (*m_RStartIter)->ROpt.IsDefferdAndForward)
		{
			(*m_RStartIter)->RenderUpdate();
			(*m_RStartIter)->TransUpdate(_Camera);
			(*m_RStartIter)->Render(_Camera);
			DEFFERD3DMAT->Update();
			(*m_RStartIter)->MeshUpdate();
			(*m_RStartIter)->RenderEndUpdate();
		}
	}
}

void HRenderMgr::Render_Defferd_Light(HPTR<HCamera> _Camera, int _Group)
{
	HPTR<HMultiRenderTaget> LIGHTTAGET = HResMgr<HMultiRenderTaget>::Find(L"LIGHT");
	LIGHTTAGET->Clear();
	LIGHTTAGET->OMSet();

	m_LightStartIter = m_LightSet.begin();
	m_LightEndIter = m_LightSet.end();

	for (; m_LightStartIter != m_LightEndIter; ++m_LightStartIter)
	{
		if (true == (*m_LightStartIter)->IsLight(_Group))
		{
			HPTR<HLight> Ptr = *m_LightStartIter;
			Ptr->CalLightData(_Camera);
			Ptr->LightRender(_Camera);
			HVAR::MainDevice().SetDsMode(L"LIGHTDEPTH");
		}
	}
}

void HRenderMgr::Render_Forward(HPTR<HCamera> _Camera, std::map<int, std::list<HPTR<HRenderer>>>::iterator _Iter, size_t _Index)
{
	m_RStartIter = m_GFindIter->second.begin();
	m_REndIter = m_GFindIter->second.end();
	LightCheck(_Camera, _Camera->m_RenderGroup[_Index]);
	for (; m_RStartIter != m_REndIter; m_RStartIter++)
	{
		if (0 == (*m_RStartIter)->ROpt.IsDefferdAndForward)
		{
			(*m_RStartIter)->RenderUpdate();
			(*m_RStartIter)->TransUpdate(_Camera);
			(*m_RStartIter)->Render(_Camera);
			(*m_RStartIter)->MatUpdate();
			(*m_RStartIter)->MeshUpdate();
			(*m_RStartIter)->RenderEndUpdate();
		}
	}
}

void HRenderMgr::ScreenMerge()
{
	m_CamStartIter = m_CameraMap.begin();
	m_CamEndIter = m_CameraMap.end();

	for (; m_CamStartIter != m_CamEndIter; ++m_CamStartIter)
	{
		m_CamStartIter->second->ScreenMerge();
	}
}
