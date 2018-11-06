#include "RenderManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "KMacro.h"
#include "Light.h"
#include "ResourceManager.h"
#include "Sampler.h"
#include "DebugManager.h"
#include "RenderTarget_Multi.h"
#include "Core_Class.h"

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}

// 순서로 처리하는 식으로 바뀔수도 2d에서만
bool RenderManager::Sort_Z(KPtr<Renderer> _Left, KPtr<Renderer> _Right)
{
	if (_Left->WSubPivot().z < _Right->WSubPivot().z)
	{
		return true;
	}

	return false;
}

void RenderManager::Reset_SList() 
{
	KPtr<Sampler> Smp = ResourceManager<Sampler>::Find(L"DefaultSmp");

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


void RenderManager::Render() 
{
	// 렌더 메니저에서 다함
	Core_Class::MainDevice().ClearTaget();


	// 첫 초기화
	Core_Class::MainDevice().OMSet(); // 0

	Reset_SList();

	m_CSI = m_CamMap.begin();
	m_CEI = m_CamMap.end();

	// 카메라 별
	for (; m_CSI != m_CEI; ++m_CSI)
	{
		// 해당 레이어가 카메라에 없으면 넘긴다.
		for (size_t i = 0; i < m_CSI->second->m_Layer.size(); ++i)
		{
			m_ALLFI = m_RenderMap.find(m_CSI->second->m_Layer[i]);

			if (m_ALLFI == m_RenderMap.end())
			{
				continue;
			}
			
			
			// 디퍼드타겟 그린다.
			// 기본 뎁스 - 덮어그리는 식
			Core_Class::MainDevice().Set_DSS(L"BASIC");
			Render_Defferd(m_CSI->second, m_ALLFI, i);

			// 라이트 연산을 한다.
			// 디퍼드는 끈다는 개념을 하던가 해야한다.
			// 라이트 뎁스 - 섞어 그림
			Core_Class::MainDevice().Set_DSS(L"LIGHTDEPTH");
			Render_DefLight(m_CSI->second, (int)i);


			// 머지 전 한 번 비운다.
			m_CSI->second->m_CameraTaget->Clear();
			m_CSI->second->m_CameraTaget->OMSet();
			m_CSI->second->Merge_Light();

			// 타겟 머지
			// 다시 켜야되.
			// 기본 뎁스 - 덮어그리는 식
			Core_Class::MainDevice().Set_DSS(L"BASIC");
			Render_Forward(m_CSI->second, m_ALLFI, i);

			// 최종 나온 결과물을 


		} // for (; m_GroupStartIter != m_GroupEndIter; ++m_GroupStartIter)
	} // for (; m_CSI != m_CEI; ++m_CSI)

	Core_Class::MainDevice().OMSet();
	Merge_Screen();
	

	if (DebugManager::Is_Debug())
	{
		// 기본 뎁스 - 완전 무조건 덮어그리는 식
		Core_Class::MainDevice().Set_DSS(L"ALWAYS");
		Core_Class::MainScene()->DebugRender();
		DebugManager::Targetting();
		DebugManager::Logging();
	}

	
	Core_Class::MainDevice().Present();
	// 카메라의 랜더타겟들을 합쳐서
	// 백버퍼 텍스처에 그릴 것이다.

}

void RenderManager::Insert_Cam(Camera* _Camera)
{
	KASSERT(nullptr == _Camera);
	m_CamMap.insert(std::map<int, KPtr<Camera>>::value_type(_Camera->order(), _Camera));
}

void RenderManager::Insert_Renderer(Renderer* _Renderer) 
{
	KASSERT(nullptr == _Renderer);

	m_ALLFI = m_RenderMap.find(_Renderer->order());

	if (m_ALLFI == m_RenderMap.end())
	{
		m_RenderMap.insert(std::unordered_map<int, std::list<KPtr<Renderer>>>::value_type(_Renderer->order(), std::list<KPtr<Renderer>>()));
		m_ALLFI = m_RenderMap.find(_Renderer->order());
	}

	m_ALLFI->second.push_back(_Renderer);
}


void RenderManager::Insert_AbsRenderer(KPtr<Renderer> _Renderer)
{
	KASSERT(nullptr == _Renderer);

	m_ALLFI = m_RenderMap.find(_Renderer->order());

	if (m_ALLFI == m_RenderMap.end())
	{
		m_RenderMap.insert(std::unordered_map<int, std::list<KPtr<Renderer>>>::value_type(_Renderer->order(), std::list<KPtr<Renderer>>()));
		m_ALLFI = m_RenderMap.find(_Renderer->order());
	}

	m_ALLFI->second.push_back(_Renderer);
}

void RenderManager::Release() {
	m_ALLRSI = m_RenderMap.begin();
	m_ALLREI = m_RenderMap.end();

	for (; m_ALLRSI != m_ALLREI; ++m_ALLRSI)
	{
		m_RSI = m_ALLRSI->second.begin();
		m_REI = m_ALLRSI->second.end();

		for (; m_RSI != m_REI; )
		{
			if (true == (*m_RSI)->Is_Death())
			{
				m_RSI = m_ALLRSI->second.erase(m_RSI);
				// m_REI = m_ALLRSI->second.end();
			}
			else {
				++m_RSI;
			}
		}
	}
}




/****************** Light *****************/
void RenderManager::Insert_Light(Light* _Light) 
{
	m_LightSet.insert(_Light);
}

// 렌더메니저에서 이걸 돌릴지 쉐이더에서 빛을 알아야 할지
// 최종적으로 픽셀이든 버텍스든 쉐이더에서 빛이 합산되어 넘어가야하기 때문
void RenderManager::Check_Light(KPtr<Camera> _Camera, int _Group)
{
	m_LSI = m_LightSet.begin();
	m_LEI = m_LightSet.end();

	int Count = 0;
	Light::LightCBDATA Data;
	memset(&Data, 0, sizeof(Light::LightCBDATA));

	for (; m_LSI != m_LEI; ++m_LSI)
	{
		if (true == (*m_LSI)->IsLight(_Group))
		{
			KPtr<Light> Ptr = (*m_LSI);
			Ptr->Set_LightData(_Camera);
			Data.ArrLight[Count] = (*m_LSI)->Data;

			++Count;
			if (10 <= Count)
			{
				break;
			}
		}
	}

	Data.LightCount = Count;

	Core_Class::MainDevice().SettingCB<Light::LightCBDATA>(L"LIGHTDATA", Data, SHTYPE::ST_VS);
	Core_Class::MainDevice().SettingCB<Light::LightCBDATA>(L"LIGHTDATA", Data, SHTYPE::ST_PS);

	// KDevice::SettingCB()

	// 이건 어느 쉐이더에도 속하지 않는 아주. 글로벌한 녀석이어야 한다.
	// 상수버퍼에 세팅해줘야 하는데 전역 상수버퍼가 없다.

	return;
}

void RenderManager::Render_Defferd(KPtr<Camera> _Camera, std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index)
{
	// 디퍼드용 메테리얼로 
	KPtr<RenderTarget_Multi> DEFFERDTAGET = ResourceManager<RenderTarget_Multi>::Find(L"DEFFERD");
	DEFFERDTAGET->Clear();
	DEFFERDTAGET->OMSet();


	m_RSI = m_ALLFI->second.begin();
	m_REI = m_ALLFI->second.end();
	for (; m_RSI != m_REI; m_RSI++)
	{
		if (1 == (*m_RSI)->ROpt.Defferd_orForward)
		{
			(*m_RSI)->Update_Trans(_Camera);
			(*m_RSI)->Update_TransCB();
			(*m_RSI)->RenderUpdate();

			// 기본 베잇6ㅡ - 그냥 그리면 된다. -> 데이터가 곧 넘어가서 계산될 값이기 때문
			// 무엇이 계산되냐... 그 있잖아 값들
			if (RENDER_DATATYPE::RDT_BASE == (*m_RSI)->ROpt.Render_DT)
			{
				for (KUINT i = 0; i < (KUINT)(*m_RSI)->Count_Mesh(); i++)
				{
					for (KUINT j = 0; j < (KUINT)(*m_RSI)->Count_Material(); j++)
					{
						(*m_RSI)->Render(_Camera);
						(*m_RSI)->Update_TexSmp(j);
						(*m_RSI)->Update_MtlCB(j);
						(*m_RSI)->Update_Material(j);
						(*m_RSI)->Update_Mesh(i);

					}
				}
				

			}
			else if (RENDER_DATATYPE::RDT_DATA == (*m_RSI)->ROpt.Render_DT)
			{
				for (KUINT i = 0; i < (KUINT)(*m_RSI)->m_RDVec.size(); i++)
				{
					Render_Data tData = (*m_RSI)->m_RDVec[i];

					(*m_RSI)->Render(_Camera);
					(*m_RSI)->Update_TexSmp((*m_RSI)->m_RDVec[i].Material);
					(*m_RSI)->Update_MtlCB((*m_RSI)->m_RDVec[i].Material);
					(*m_RSI)->Update_Material((*m_RSI)->m_RDVec[i].Material);
					(*m_RSI)->Update_SelectMesh((*m_RSI)->m_RDVec[i].Mesh
						, (*m_RSI)->m_RDVec[i].Vertex
						, (*m_RSI)->m_RDVec[i].Sub_inx);
				}

			}

			(*m_RSI)->RenderFinalUpdate();
		}
	}
}

void RenderManager::Render_Forward(KPtr<Camera> _Camera, std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index)
{
	m_RSI = m_ALLFI->second.begin();
	m_REI = m_ALLFI->second.end();
	Check_Light(_Camera, _Camera->m_Layer[_Index]);
	for (; m_RSI != m_REI; m_RSI++)
	{
		if (0 == (*m_RSI)->ROpt.Defferd_orForward)
		{
			(*m_RSI)->Update_Trans(_Camera);
			(*m_RSI)->Update_TransCB();
			(*m_RSI)->RenderUpdate();

			// 기본 베잇6ㅡ - 그냥 그리면 된다. -> 데이터가 곧 넘어가서 계산될 값이기 때문
			// 무엇이 계산되냐... 그 있잖아 값들
			if (RENDER_DATATYPE::RDT_BASE == (*m_RSI)->ROpt.Render_DT)
			{
				for (KUINT i = 0; i < (KUINT)(*m_RSI)->Count_Mesh(); i++)
				{
					for (KUINT j = 0; j < (KUINT)(*m_RSI)->Count_Material(); j++)
					{
						(*m_RSI)->Render(_Camera);
						(*m_RSI)->Update_TexSmp(j);
						(*m_RSI)->Update_MtlCB(j);
						(*m_RSI)->Update_Material(j);
						(*m_RSI)->Update_Mesh(i);

					}
				}
			}
			else if (RENDER_DATATYPE::RDT_DATA == (*m_RSI)->ROpt.Render_DT)
			{
				for (KUINT i = 0; i < (KUINT)(*m_RSI)->m_RDVec.size(); i++)
				{
					Render_Data tData = (*m_RSI)->m_RDVec[i];

					(*m_RSI)->Render(_Camera);
					(*m_RSI)->Update_TexSmp((*m_RSI)->m_RDVec[i].Material);
					(*m_RSI)->Update_MtlCB((*m_RSI)->m_RDVec[i].Material);
					(*m_RSI)->Update_Material((*m_RSI)->m_RDVec[i].Material);

					(*m_RSI)->Update_SelectMesh((*m_RSI)->m_RDVec[i].Mesh
						, (*m_RSI)->m_RDVec[i].Vertex
						, (*m_RSI)->m_RDVec[i].Sub_inx);
				}

			}

			(*m_RSI)->RenderFinalUpdate();

		}
	}
}


void RenderManager::Render_DefLight(KPtr<Camera> _Camera, int _Group)
{
	KPtr<RenderTarget_Multi> LIGHTTAGET = ResourceManager<RenderTarget_Multi>::Find(L"LIGHT");
	LIGHTTAGET->Clear();
	LIGHTTAGET->OMSet();

	m_LSI = m_LightSet.begin();
	m_LEI = m_LightSet.end();

	for (; m_LSI != m_LEI; ++m_LSI)
	{
		if (true == (*m_LSI)->IsLight(_Group))
		{
			KPtr<Light> Ptr = *m_LSI;
			Ptr->Set_LightData(_Camera);
			Ptr->Render_Light(_Camera);
			Core_Class::MainDevice().Set_DSS(L"LIGHTDEPTH");
		}
	}
}

void RenderManager::Merge_Screen()
{
	m_CSI = m_CamMap.begin();
	m_CEI = m_CamMap.end();

	for (; m_CSI != m_CEI; ++m_CSI)
	{
		m_CSI->second->Merge_Screen();
	}
}
