#pragma once
#include <vector>
#include "DXContainer.h"
#include "Component_DE.h"
#include "RenderTarget_Multi.h"

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
	void Merge_Light(); 
	void Merge_Screen();

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
	// 카메라 필터.

public:
	Camera();
	~Camera();
};

