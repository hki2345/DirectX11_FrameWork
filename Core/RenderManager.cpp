	#include "RenderManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "KMacro.h"
#include "KLight.h"
#include "Sampler.h"
#include "Core_Class.h"

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
	m_CameraMap.insert(_Camera);
}
void RenderManager::Insert_Renderer(Renderer* _Renderer)
{
	KASSERT(nullptr == _Renderer);

	m_Renderer_FindIter = m_RendererMap.find(_Renderer->order());

	// �ش� ���̾�(����Ʈ)�� ������ ����� �ְ� �ű⿡ ����
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
	Reset_SamplerList();

	m_Camera_StartIter = m_CameraMap.begin();
	m_Camera_EndIter = m_CameraMap.end();

	// ī�޶� ��
	for (; m_Camera_StartIter != m_Camera_EndIter; ++m_Camera_StartIter)
	{
		// ���̾� ��
		for (size_t i = 0; i < (*m_Camera_StartIter)->m_Layer.size(); ++i)
		{
			m_Renderer_FindIter = m_RendererMap.find((*m_Camera_StartIter)->m_Layer[i]);

			// �ش� ���̾ ī�޶� ������ �ѱ��.
			if (m_Renderer_FindIter == m_RendererMap.end())
			{
				continue;
			}

			// ���⿡ ������ 2 �̻��̸� �ڵ� �����ϰ� �ƴµ� ����
			// ����ڰ� ���� ������ ������ ���θ� ������

			m_Renderer_StartIter = m_Renderer_FindIter->second.begin();
			m_Renderer_EndIter = m_Renderer_FindIter->second.end();

			// ���⿡ ����Ʈ�� ��Ƽ� ������ۿ� �ѹ��� ����
			Light_Check((*m_Camera_StartIter)->m_Layer[i], m_Camera_StartIter);

			for (; m_Renderer_StartIter != m_Renderer_EndIter; ++m_Renderer_StartIter)
			{
				(*m_Renderer_StartIter)->RenderUpdate();
				(*m_Renderer_StartIter)->Update_Trans((*m_Camera_StartIter));
				(*m_Renderer_StartIter)->Render((*m_Camera_StartIter));
				(*m_Renderer_StartIter)->Update_MeshMat();
				(*m_Renderer_StartIter)->RenderFinalUpdate();
			}
		}		
	}
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

// �����޴������� �̰� ������ ���̴����� ���� �˾ƾ� ����
// ���������� �ȼ��̵� ���ؽ��� ���̴����� ���� �ջ�Ǿ� �Ѿ���ϱ� ����
void RenderManager::Light_Check(const int& _Layer, const std::set<KPtr<Camera>>::iterator& _Iter)
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

			// ���Ⱚ�� �������� �ϱ� ������ -> Zero�� �����ش�.
			TempData.ArrLight[Cnt].m_Dir = -(*_Iter)->View().Multi_Vector_Z(TempData.ArrLight[Cnt].m_Dir);
			TempData.ArrLight[Cnt].m_Pos = -(*_Iter)->View().Multi_Vector_Z(TempData.ArrLight[Cnt].m_Pos);
			TempData.ArrLight[Cnt].CamPos = -(*_Iter)->View().Multi_Vector_Z(TempData.ArrLight[Cnt].CamPos);
			++Cnt;

			if (10 <= Cnt)
			{
				break;
			}
		}
	}

	WLOG(L"Light View Pos: %f, %f, %f", TempData.ArrLight[0].m_Pos.x, TempData.ArrLight[0].m_Pos.y, TempData.ArrLight[0].m_Pos.z);
	WLOG(L"View Cam Pos: %f, %f, %f", TempData.ArrLight[0].CamPos.x, TempData.ArrLight[0].CamPos.y, TempData.ArrLight[0].CamPos.z);
	TempData.LCnt = Cnt;

	Core_Class::MainDevice().Set_DeviceCB<KLight::LightCB>(L"LIGHT_DATA", TempData, SHADER_TYPE::ST_VS);
	Core_Class::MainDevice().Set_DeviceCB<KLight::LightCB>(L"LIGHT_DATA", TempData, SHADER_TYPE::ST_PS);
}