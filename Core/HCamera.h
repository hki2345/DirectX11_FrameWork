#pragma once
#include <vector>
#include "DXContainer.h"
#include "HBaseCom.h"
#include "HMultiRenderTaget.h"

class HMesh;
class HMaterial;
class HTrans;
class HRenderMgr;
class HCamera final : public HBaseCom
{
public:
	friend HRenderMgr;
	friend class DebugManager;

public:
	enum PROJMODE
	{
		PM_PERS,
		PM_ORTH
	};

	enum SIZEMODE
	{
		SM_WINDOW,
		SM_CUSTOM,
	};

private:
	int      m_Order;
	PROJMODE m_ePMode;

	KMatrix m_View; // 카메라
	KMatrix m_Proj; // 투영
	KMatrix m_VP; // 투영

	SIZEMODE m_eSMode;
	KVector2 m_ScreenSize;
	float m_ScreenRatioX;
	float m_ScreenRatioY;
	float m_fFov;
	float m_fNear;
	float m_fFar;

private:
	std::vector<int> m_RenderGroup;


public:
	int Order() {
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

	const KMatrix& CV() const
	{
		return m_View;
	}

	const KMatrix& CP() const
	{
		return m_Proj;
	}

	const KMatrix& VP() const
	{
		return m_VP;
	}

public:
	bool Init(int _Order = 0);

public:
	template<typename ...Rest>
	void PushRenderLayer(Rest ...Arg) 
	{
		PushLayer(Arg...);
	}

	KVector4 ScreenToWorld(KVector2 _ScreenPos);

private:
	template<typename ...Rest>
	void PushLayer(int _Data, Rest ...Arg)
	{
		m_RenderGroup.push_back(_Data);
		PushLayer(Arg...);
	}

	void PushLayer() {	}

public:
	void ProjectionMode(PROJMODE _Mode)
	{
		m_ePMode = _Mode;
	}

	void ModeChange() 
	{
		if (m_ePMode == PROJMODE::PM_PERS)
		{
			m_ePMode = PROJMODE::PM_ORTH;
		}
		else {
			m_ePMode = PROJMODE::PM_PERS;
		}
	}

	void ScreenSize(KVector2 _ScreenSize) {
		m_ScreenSize = _ScreenSize;
		m_eSMode = SM_CUSTOM;
	}
	
	KVector2 ScreenSize() {
		return m_ScreenSize;
	}

private:
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void EndUpdate() override;



private: // CameraRender
	KPtr<HMesh> m_CamMesh;
	KPtr<HMaterial> m_CamMat;
	KPtr<HMaterial> m_CamScreenMat;

private:
	void LightMerge(); 
	void ScreenMerge();

private:
	KPtr<HMultiRenderTaget> m_CameraTaget;
	// 카메라 필터.

public:
	HCamera();
	~HCamera();
};

