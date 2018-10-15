	#include "RenderManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "KMacro.h"
#include "KLight.h"
#include "Sampler.h"
#include "Core_Class.h"

#include "RenderTarget_Multi.h"
#include "DebugManager.h"
#include "ResourceManager.h"

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}

bool RenderManager::Sort_Z(KPtr<Renderer> _Left, KPtr<Renderer> _Right)
{
	if (_Left->pivot_world().z > _Right->pivot_world().z)
	{
		return true;
	}

	return false;
}


void RenderManager::Insert_Camera(Camera* _Camera)
{
	KASSERT(nullptr == _Camera);
	m_CameraMap.insert(std::map<int, KPtr<Camera>>::value_type(_Camera->order(), _Camera));
}
void RenderManager::Insert_Renderer(Renderer* _Renderer)
{
	KASSERT(nullptr == _Renderer);

	m_Renderer_FindIter = m_RendererMap.find(_Renderer->order());

	// 해당 레이어(리스트)가 없으면 만들어 주고 거기에 삽입
	if (m_Renderer_FindIter == m_RendererMap.end())
	{
		m_RendererMap.insert(
			std::unordered_map<int, std::list<KPtr<Renderer>>>::value_type(_Renderer->order(), std::list<KPtr<Renderer>>()));
		m_Renderer_FindIter = m_RendererMap.find(_Renderer->order());
	}

	m_Renderer_FindIter->second.push_back(_Renderer);
}

void RenderManager::insert_AbsRenderer(KPtr<Renderer> _Renderer)
{
	KASSERT(nullptr == _Renderer);

	m_Renderer_FindIter = m_RendererMap.find(_Renderer->order());

	if (m_Renderer_FindIter == m_RendererMap.end())
	{
		m_RendererMap.insert(std::unordered_map<int, std::list<KPtr<Renderer>>>::
			value_type(_Renderer->order(), std::list<KPtr<Renderer>>()));
	}

	m_Renderer_FindIter = m_RendererMap.find(_Renderer->order());
	m_Renderer_FindIter->second.push_back(_Renderer);
}


void RenderManager::Reset_SamplerList()
{
	KPtr<Sampler> FS = ResourceManager<Sampler>::Find(L"DefaultSam");
	
	KASSERT(nullptr == FS);

	for (UINT i = 0; i < 8; i++)
	{
		FS->Update(i);
	}
}

void RenderManager::Render()
{
	// 렌더 메니저에서 다함
	Core_Class::MainDevice().Clear_Target();
	
	// 첫 초기화
	Core_Class::MainDevice().SetOM();

	Reset_SamplerList();

	m_Camera_StartIter = m_CameraMap.begin();
	m_Camera_EndIter = m_CameraMap.end();

	

	// 카메라 별
	for (; m_Camera_StartIter != m_Camera_EndIter; ++m_Camera_StartIter)
	{
		// 레이어 별
		for (size_t i = 0; i < m_Camera_StartIter->second->m_Layer.size(); ++i)
		{
			m_Renderer_FindIter = m_RendererMap.find(m_Camera_StartIter->second->m_Layer[i]);

			// 해당 레이어가 카메라에 없으면 넘긴다.
			if (m_Renderer_FindIter == m_RendererMap.end())
			{
				continue;
			}

			Render_Defferd(m_Renderer_FindIter, i);
			Render_LightDef((int)i, m_Camera_StartIter->second);

			// 라이트 연산 후 -> 카메라에 모두 찍어냄
			m_Camera_StartIter->second->m_MTarget->Clear();
			m_Camera_StartIter->second->m_MTarget->SetOM();
			m_Camera_StartIter->second->Render_Light();

			Render_Forward(m_Renderer_FindIter, i);
		}		
	}

	if (true == DebugManager::Is_Debug())
	{
		Core_Class::MainDevice().SetOM_Deg();
		Core_Class::main_state()->DebugRender();
		DebugManager::Targetting();
		DebugManager::Logging();
	}

	Core_Class::MainDevice().Present();
}

void RenderManager::Release()
{
	m_AllRenderer_StartIter = m_RendererMap.begin();
	m_AllRenderer_EndIter = m_RendererMap.end();

	for (; m_AllRenderer_StartIter != m_AllRenderer_EndIter; ++m_AllRenderer_StartIter)
	{
		m_Renderer_StartIter = m_AllRenderer_StartIter->second.begin();
		m_Renderer_EndIter = m_AllRenderer_StartIter->second.end();

		for (; m_Renderer_StartIter != m_Renderer_EndIter;)
		{		
			if (true == (*m_Renderer_StartIter)->is_Death())
			{
				m_Renderer_StartIter = m_AllRenderer_StartIter->second.erase(m_Renderer_StartIter);
			}

			else
			{
				++m_Renderer_StartIter;
			}
		}
	}
}

bool RenderManager::Is_Name(const wchar_t* _Name)
{
	m_AllRenderer_StartIter = m_RendererMap.begin();
	m_AllRenderer_EndIter = m_RendererMap.end();

	for (; m_AllRenderer_StartIter != m_AllRenderer_EndIter; ++m_AllRenderer_StartIter)
	{
		m_Renderer_StartIter = m_AllRenderer_StartIter->second.begin();
		m_Renderer_EndIter = m_AllRenderer_StartIter->second.end();

		for (; m_Renderer_StartIter != m_Renderer_EndIter; ++m_Renderer_StartIter)
		{
			std::wstring OneName = (*m_Renderer_StartIter)->name();
			if (OneName == _Name && false == (*m_Renderer_StartIter)->is_Death())
			{
				return true;
			}
		}
	}

	return false;
}


/****************** Light *****************/
void RenderManager::Insert_Light(KLight* _Light)
{
	m_LightSet.insert(_Light);
}

// 렌더메니저에서 이걸 돌릴지 쉐이더에서 빛을 알아야 할지
// 최종적으로 픽셀이든 버텍스든 쉐이더에서 빛이 합산되어 넘어가야하기 때문
void RenderManager::Light_Check(const int& _Layer, KPtr<Camera> _Iter)
{
	m_LS = m_LightSet.begin();
	m_LE = m_LightSet.end();

	int Cnt = 0;
	KLight::LightCB TempData;
	memset(&TempData, 0, sizeof(KLight::LightCB));

	for (; m_LS != m_LE; ++m_LS)
	{
		if (true == (*m_LS)->Is_Focus(_Layer))
		{
			TempData.ArrLight[Cnt] = (*m_LS)->m_LD;

			// 방향값만 곱해져야 하기 때문예 -> Zero로 곱해준다.
			TempData.ArrLight[Cnt].m_Dir = _Iter->View().Multi_Vector_Z(TempData.ArrLight[Cnt].m_Dir);
			TempData.ArrLight[Cnt].m_Pos = _Iter->View().Multi_Vector_Z(TempData.ArrLight[Cnt].m_Pos);
			TempData.ArrLight[Cnt].CamPos = _Iter->View().Multi_Vector_Z(TempData.ArrLight[Cnt].CamPos);
			++Cnt;

			if (10 <= Cnt)
			{
				break;
			}
		}
	}

	TempData.LCnt = Cnt;

	Core_Class::MainDevice().Set_DeviceCB<KLight::LightCB>(L"LIGHT_DATA", TempData, SHADER_TYPE::ST_VS);
	Core_Class::MainDevice().Set_DeviceCB<KLight::LightCB>(L"LIGHT_DATA", TempData, SHADER_TYPE::ST_PS);

	return;
}


void RenderManager::Render_Defferd(std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index)
{
	// 디퍼드용 메테리얼로 
	KPtr<RenderTarget_Multi> DEFFERDTARGET = ResourceManager<RenderTarget_Multi>::Find(L"DEFFERD");
	DEFFERDTARGET->Clear();
	DEFFERDTARGET->SetOM();

	KPtr<Material> DEFFERDMAT = ResourceManager<Material>::Find(L"DEFFERD_MAT");
	KASSERT(nullptr == DEFFERDMAT);
	

	m_Renderer_StartIter = m_Renderer_FindIter->second.begin();
	m_Renderer_EndIter = m_Renderer_FindIter->second.end();
	Light_Check((*m_Camera_StartIter)->m_Layer[_Index], m_Camera_StartIter);

	for (; m_Renderer_StartIter != m_Renderer_EndIter; m_Renderer_StartIter++)
	{
		if (1 == (*m_Renderer_StartIter)->m_ROption.Deffert_orFoward)
		{
			(*m_Renderer_StartIter)->RenderUpdate();
			(*m_Renderer_StartIter)->Update_Trans((*m_Camera_StartIter));
			(*m_Renderer_StartIter)->Render((*m_Camera_StartIter));
			DEFFERDMAT->Update();
			(*m_Renderer_StartIter)->Update_Mesh();
			(*m_Renderer_StartIter)->RenderFinalUpdate();
		}
	}
}

void RenderManager::Render_Forward(std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index)
{
	// 포워드는 그냥 메인에 그린다. 
	// Core_Class::MainDevice().Clear_Target();
	// Core_Class::MainDevice().SetOM();

	m_Renderer_StartIter = m_Renderer_FindIter->second.begin();
	m_Renderer_EndIter = m_Renderer_FindIter->second.end();
	Light_Check((*m_Camera_StartIter)->m_Layer[_Index], m_Camera_StartIter);
	for (; m_Renderer_StartIter != m_Renderer_EndIter; m_Renderer_StartIter++)
	{
		if (0 == (*m_Renderer_StartIter)->m_ROption.Deffert_orFoward)
		{
			(*m_Renderer_StartIter)->RenderUpdate();
			(*m_Renderer_StartIter)->Update_Trans((*m_Camera_StartIter));
			(*m_Renderer_StartIter)->Render((*m_Camera_StartIter));
			(*m_Renderer_StartIter)->Update_Material();
			(*m_Renderer_StartIter)->Update_Mesh();
			(*m_Renderer_StartIter)->RenderFinalUpdate();
		}
	}
}

void RenderManager::Render_LightDef(const int& _Layer, const std::set<KPtr<Camera>>::iterator& _Iter)
{
	KPtr<RenderTarget_Multi> LIGHTTARGET = ResourceManager<RenderTarget_Multi>::Find(L"LIGHT");
	LIGHTTARGET->Clear();
	LIGHTTARGET->SetOM();

	m_LS = m_LightSet.begin();
	m_LE = m_LightSet.end();

	for (; m_LS != m_LE; ++m_LS)
	{
		if (true == (*m_LS)->Is_Focus(_Layer))
		{
			KPtr<KLight> LPtr = *m_LS;
			LPtr->Render();
		}
	}
}