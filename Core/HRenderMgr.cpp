#include "HRenderMgr.h"
#include "HRenderer.h"
#include "HCamera.h"
#include "KMacro.h"
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

// ������ ó���ϴ� ������ �ٲ���� 2d������
bool HRenderMgr::ZOrderSort(KPtr<HRenderer> _Left, KPtr<HRenderer> _Right)
{
	if (_Left->WSubPivot().z < _Right->WSubPivot().z)
	{
		return true;
	}

	return false;
}

void HRenderMgr::ResetSR() 
{
	KPtr<HSampler> Smp = HResMgr<HSampler>::Find(L"DefaultSmp");

	if (nullptr == Smp)
	{
		BBY;
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
			// ���۵�Ÿ�� �׸���.
			Render_Defferd(m_CamStartIter->second, m_GFindIter, i);
			// ����Ʈ ������ �Ѵ�.

			HVAR::MainDevice().SetDsMode(L"LIGHTDEPTH");
			// ���۵�� ���ٴ� ������ �ϴ��� �ؾ��Ѵ�.
			Render_Defferd_Light(m_CamStartIter->second, (int)i);

			m_CamStartIter->second->m_CameraTaget->Clear();
			m_CamStartIter->second->m_CameraTaget->OMSet();
			m_CamStartIter->second->LightMerge();
			// Ÿ�� ����

			HVAR::MainDevice().SetDsMode(L"BASIC");
			// �ٽ� �Ѿߵ�.
			Render_Forward(m_CamStartIter->second, m_GFindIter, i);

			// ���� ���� ������� 

			// ī�޶� �ϳ� ����Ÿ���� ������ ���� ���̴�.
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
	// ī�޶��� ����Ÿ�ٵ��� ���ļ�
	// ����� �ؽ�ó�� �׸� ���̴�.

}

void HRenderMgr::PushCamera(HCamera* _Camera)
{
	KASSERT(nullptr == _Camera);
	m_CameraMap.insert(std::map<int, KPtr<HCamera>>::value_type(_Camera->Order(), _Camera));
}

void HRenderMgr::PushRenderer(HRenderer* _Renderer) 
{
	KASSERT(nullptr == _Renderer);

	m_GFindIter = m_RenderMap.find(_Renderer->Order());

	if (m_GFindIter == m_RenderMap.end())
	{
		m_RenderMap.insert(std::unordered_map<int, std::list<KPtr<HRenderer>>>::value_type(_Renderer->Order(), std::list<KPtr<HRenderer>>()));
		m_GFindIter = m_RenderMap.find(_Renderer->Order());
	}

	m_GFindIter->second.push_back(_Renderer);
}


void HRenderMgr::PushOverRenderer(KPtr<HRenderer> _Renderer)
{
	KASSERT(nullptr == _Renderer);

	m_GFindIter = m_RenderMap.find(_Renderer->Order());

	if (m_GFindIter == m_RenderMap.end())
	{
		m_RenderMap.insert(std::unordered_map<int, std::list<KPtr<HRenderer>>>::value_type(_Renderer->Order(), std::list<KPtr<HRenderer>>()));
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

void HRenderMgr::LightCheck(KPtr<HCamera> _Camera, int _Group)
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
			KPtr<HLight> Ptr = (*m_LightStartIter);
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

	// �̰� ��� ���̴����� ������ �ʴ� ����. �۷ι��� �༮�̾�� �Ѵ�.
	// ������ۿ� ��������� �ϴµ� ���� ������۰� ����.

	return;
}

void HRenderMgr::Render_Defferd(KPtr<HCamera> _Camera, std::map<int, std::list<KPtr<HRenderer>>>::iterator _Iter, size_t _Index)
{
	// ���۵�� ���׸���� 
	KPtr<HMultiRenderTaget> DEFFERDTAGET = HResMgr<HMultiRenderTaget>::Find(L"DEFFERD");
	DEFFERDTAGET->Clear();
	DEFFERDTAGET->OMSet();

	KPtr<HMaterial> DEFFERD3DMAT = HResMgr<HMaterial>::Find(L"DEFFERD3DMAT");
	KASSERT(nullptr == DEFFERD3DMAT);
	if (nullptr == DEFFERD3DMAT)
	{
		BBY;
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

void HRenderMgr::Render_Defferd_Light(KPtr<HCamera> _Camera, int _Group)
{
	KPtr<HMultiRenderTaget> LIGHTTAGET = HResMgr<HMultiRenderTaget>::Find(L"LIGHT");
	LIGHTTAGET->Clear();
	LIGHTTAGET->OMSet();

	m_LightStartIter = m_LightSet.begin();
	m_LightEndIter = m_LightSet.end();

	for (; m_LightStartIter != m_LightEndIter; ++m_LightStartIter)
	{
		if (true == (*m_LightStartIter)->IsLight(_Group))
		{
			KPtr<HLight> Ptr = *m_LightStartIter;
			Ptr->CalLightData(_Camera);
			Ptr->LightRender(_Camera);
			HVAR::MainDevice().SetDsMode(L"LIGHTDEPTH");
		}
	}
}

void HRenderMgr::Render_Forward(KPtr<HCamera> _Camera, std::map<int, std::list<KPtr<HRenderer>>>::iterator _Iter, size_t _Index)
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
