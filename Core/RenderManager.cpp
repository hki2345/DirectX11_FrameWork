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

// ������ ó���ϴ� ������ �ٲ���� 2d������
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
	// ���� �޴������� ����
	Core_Class::MainDevice().ClearTaget();


	// ù �ʱ�ȭ
	Core_Class::MainDevice().OMSet(); // 0

	Reset_SList();

	m_CSI = m_CamMap.begin();
	m_CEI = m_CamMap.end();

	// ī�޶� ��
	for (; m_CSI != m_CEI; ++m_CSI)
	{
		// �ش� ���̾ ī�޶� ������ �ѱ��.
		for (size_t i = 0; i < m_CSI->second->m_Layer.size(); ++i)
		{
			m_ALLFI = m_RenderMap.find(m_CSI->second->m_Layer[i]);

			if (m_ALLFI == m_RenderMap.end())
			{
				continue;
			}
			
			
			// ���۵�Ÿ�� �׸���.
			// �⺻ ���� - ����׸��� ��
			Core_Class::MainDevice().Set_DSS(L"BASIC");
			Render_Defferd(m_CSI->second, m_ALLFI, i);

			// ����Ʈ ������ �Ѵ�.
			// ���۵�� ���ٴ� ������ �ϴ��� �ؾ��Ѵ�.
			// ����Ʈ ���� - ���� �׸�
			Core_Class::MainDevice().Set_DSS(L"LIGHTDEPTH");
			Render_DefLight(m_CSI->second, (int)i);


			// ���� �� �� �� ����.
			m_CSI->second->m_CameraTaget->Clear();
			m_CSI->second->m_CameraTaget->OMSet();
			m_CSI->second->Merge_Light();

			// Ÿ�� ����
			// �ٽ� �Ѿߵ�.
			// �⺻ ���� - ����׸��� ��
			Core_Class::MainDevice().Set_DSS(L"BASIC");
			Render_Forward(m_CSI->second, m_ALLFI, i);

			// ���� ���� ������� 


		} // for (; m_GroupStartIter != m_GroupEndIter; ++m_GroupStartIter)
	} // for (; m_CSI != m_CEI; ++m_CSI)

	Core_Class::MainDevice().OMSet();
	Merge_Screen();
	

	if (DebugManager::Is_Debug())
	{
		// �⺻ ���� - ���� ������ ����׸��� ��
		Core_Class::MainDevice().Set_DSS(L"ALWAYS");
		Core_Class::MainScene()->DebugRender();
		DebugManager::Targetting();
		DebugManager::Logging();
	}

	
	Core_Class::MainDevice().Present();
	// ī�޶��� ����Ÿ�ٵ��� ���ļ�
	// ����� �ؽ�ó�� �׸� ���̴�.

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

// �����޴������� �̰� ������ ���̴����� ���� �˾ƾ� ����
// ���������� �ȼ��̵� ���ؽ��� ���̴����� ���� �ջ�Ǿ� �Ѿ���ϱ� ����
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

	// �̰� ��� ���̴����� ������ �ʴ� ����. �۷ι��� �༮�̾�� �Ѵ�.
	// ������ۿ� ��������� �ϴµ� ���� ������۰� ����.

	return;
}

void RenderManager::Render_Defferd(KPtr<Camera> _Camera, std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index)
{
	// ���۵�� ���׸���� 
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

			// �⺻ ����6�� - �׳� �׸��� �ȴ�. -> �����Ͱ� �� �Ѿ�� ���� ���̱� ����
			// ������ ���ǳ�... �� ���ݾ� ����
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

			// �⺻ ����6�� - �׳� �׸��� �ȴ�. -> �����Ͱ� �� �Ѿ�� ���� ���̱� ����
			// ������ ���ǳ�... �� ���ݾ� ����
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
