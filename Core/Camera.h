#pragma once
#include <vector>
#include "DXContainer.h"
#include "Component_DE.h"
#include "RenderTarget_Multi.h"

class KMaterial;
class RenderTarget_Multi;
class Effect_Post : public SmartPtr
{
private:
	bool m_OnOff;
	KPtr<RenderTarget_Multi> m_RTarget;
	KPtr<RenderTarget_Multi> m_OTarget;

public:
	bool effect_on_off() { return m_OnOff; }

	void SWITCH(const bool& _Value)
	{
		m_OnOff = _Value;
	}
	void Set_On()
	{
		m_OnOff = true;	
	}
	void Set_Off()
	{
		m_OnOff = false;
	}


	virtual void Create();
	virtual void Update();

public:
	void Set_ResourceTarget(RenderTarget_Multi* _Value)
	{
		m_RTarget = _Value;
	}

	void Set_OutTarget(RenderTarget_Multi* _Value)
	{
		m_OTarget = _Value;
	}

protected:
	Effect_Post() : m_OnOff(true), m_RTarget(nullptr), m_OTarget(nullptr)
	{

	}
};



class KMesh;
class KMaterial;
class TransPosition;
class RenderManager;
class Camera final : public Component_DE
{
public:
	friend RenderManager;
	friend class DebugManager;

public:
	enum PROJ_MODE
	{
		PM_PERS,
		PM_ORTH
	};

	enum SIZE_MODE
	{
		SM_WINDOW,
		SM_CUSTOM,
	};

private:
	// 카메라 층 별 렌더 순서를 다르게 할 수 있다. - UI
	int      m_Order;
	PROJ_MODE m_ePMode;

	KMatrix m_View; // 카메라
	KMatrix m_Proj; // 투영
	KMatrix m_VP; // 투영

	SIZE_MODE m_eSMode;
	KVector2 m_ScreenSize;
	float m_ScreenRatioX;
	float m_ScreenRatioY;
	float m_fFov;
	float m_fNear;
	float m_fFar;

private:
	std::vector<int> m_Layer;


public:
	int order() 
	{
		return m_Order;
	}

	void Near(float _fNear)
	{
		m_fNear = _fNear;
	}

	void Far(float _fFar)
	{
		m_fFar = _fFar;
	}

	const KMatrix& View() const
	{
		return m_View;
	}

	const KMatrix& Proj() const
	{
		return m_Proj;
	}

	const KMatrix& ViewProj() const
	{
		return m_VP;
	}

public:
	bool Init(int _Order = 0);

public:
	template<typename ...Rest>
	void Insert_LayerData(Rest ...Arg) 
	{
		Insert_Layer(Arg...);
	}

	KVector4 ScreenTo_World(KVector2 _ScreenPos);

private:
	template<typename ...Rest>
	void Insert_Layer(int _Data, Rest ...Arg)
	{
		m_Layer.push_back(_Data);
		Insert_Layer(Arg...);
	}

	void Insert_Layer() {	}

public:
	void ProjectionMode(PROJ_MODE _Mode)
	{
		m_ePMode = _Mode;
	}

	void Change_Mode() 
	{
		if (m_ePMode == PROJ_MODE::PM_PERS)
		{
			m_ePMode = PROJ_MODE::PM_ORTH;
		}
		else {
			m_ePMode = PROJ_MODE::PM_PERS;
		}
	}

	void screen_size(KVector2 _ScreenSize) {
		m_ScreenSize = _ScreenSize;
		m_eSMode = SM_CUSTOM;
	}
	
	KVector2 screen_size() {
		return m_ScreenSize;
	}

private:
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void EndUpdate() override;



private: // CameraRender
	KPtr<KMesh> m_CamMesh;
	KPtr<KMaterial> m_CamLightMtl; 
	KPtr<KMaterial> m_CamScreenMtl;

private:
	std::list<KPtr<Effect_Post>> m_EPList;
	std::list<KPtr<Effect_Post>>::iterator m_SFLI;
	std::list<KPtr<Effect_Post>>::iterator m_EFLI;

	std::map<std::wstring, KPtr<Effect_Post>> m_EPMap;
	std::map<std::wstring, KPtr<Effect_Post>>::iterator m_SFMI;
	std::map<std::wstring, KPtr<Effect_Post>>::iterator m_EFMI;

public:
	template<typename T>
	void Create_EffectPost(const wchar_t* _Name)
	{
		T* NPost = new T();
		if (false == NPost->Create())
		{
			BBY;
		}

		m_EPMap.insert(std::map<std::wstring, KPtr<Effect_Post>>::value_type(_Name, NPost));
	}


private:
	void Merge_Light(); 
	void Merge_Screen();
	void Progress_Post();

	void Set_Target();


	KPtr<RenderTarget_Multi> defferd_target()
	{
		return m_DefferdTarget;
	}
	KPtr<RenderTarget_Multi> light_target()
	{
		return m_LightTarget;
	}

	KPtr<KMaterial> material_camlight()
	{
		return m_CamLightMtl;
	}

private:
	KPtr<RenderTarget_Multi> m_DefferdTarget;
	KPtr<RenderTarget_Multi> m_LightTarget;
	KPtr<RenderTarget_Multi> m_CameraTaget;

	KPtr<RenderTarget_Multi> m_PostEffectTarget;
	KPtr<RenderTarget_Multi> m_PrevTarget;
	KPtr<RenderTarget_Multi> m_NextTarget;
	// 카메라 필터.

public:
	Camera();
	~Camera();
};

