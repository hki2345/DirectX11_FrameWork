#pragma once
#include <vector>
#include "DirectXHeader.h"
#include "Component_DE.h"

// ���۵� �޽� -> ������ Ÿ�ٿ� �ѹ��� �׸����� 
#include "RenderTarget_Multi.h"



class Mesh;
class Material;

// 3D ������ �ٽ�
class TransPosition;
class RenderManager;
class Camera final: public Component_DE
{
public:
	friend RenderManager;
	friend class DebugManager;

public:
	enum PROJ_MODE
	{
		PM_PERSPECTIVE,
		PM_ORTHGRAPHICS
	};

	// ������� ���� ���� �����츦 ����
	enum SIZE_MODE
	{
		SM_WINDOW,
		SM_CUSTOM
	};

private:
	
	KMatrix m_View;
	KMatrix m_Proj;
	KMatrix m_VP;

	PROJ_MODE m_ePMode;
	SIZE_MODE m_eSMode;

	KVector2 m_ScreenSize;
	float m_ScreenRatioX;
	float m_ScreenRatioY;

	float m_fFov;
	float m_fNear;
	float m_fFar;

public:
	void Far(const float& _Value)
	{
		m_fFar = _Value;	
	}

	void Near(const float& _Value)
	{
		m_fNear = _Value;
	}

	void Mode(const PROJ_MODE& _Mode)
	{
		m_ePMode = _Mode;
	}


	const KMatrix& View() const
	{
		return m_View;
	}

	const KMatrix& Proj() const
	{
		return m_Proj;
	}

	const KMatrix& View_Proj() const
	{
		return m_VP;
	}

	void screen_size(const KVector2& _Value)
	{
		m_ScreenSize = _Value;
		m_eSMode = SM_CUSTOM;
	}
	KVector2& screen_size ()
	{
		return m_ScreenSize;
	}



	// ī�޶� ���߾��� ���̾�
	// ���߿� ���� ���� ������ ���� ������ �߿��ϱ⿡
	// �ڵ����ĵǴ� �¿����� ���ͷ� ��
private:
	std::vector<int> m_Layer;
	
public:
	bool Init() override;
	KVector2 screen_to_world(const KVector2& _StatePos);

	template<typename ...Rest>
	void Insert_LayerData(Rest ...Args)
	{
		Insert_Layer(Args...);
	}

private:
	template<typename ...Rest>
	void Insert_Layer(int _Data, Rest ...Args)
	{ 
		m_Layer.push_back(_Data);
		Insert_Layer(Args...);
	}
	void Insert_Layer()  {}

private:
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void End_Update() override;


	// Forward Render
private:
	KPtr<Mesh> m_Mesh;
	KPtr<Material> m_Material;
	KPtr<RenderTarget_Multi> m_Target;



private:
	// Merge
	void Render();


public:
	Camera();
	~Camera();
};

