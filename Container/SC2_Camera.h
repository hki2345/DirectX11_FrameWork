#pragma once
#include "SomeOne.h"
#include <Camera.h>
#include <DebugManager.h>

class KMesh;
class KMaterial;
class Sampler;
class SC2_Camera : public SomeOne
{
public:
	SC2_Camera();
	~SC2_Camera();

public:
	enum SC2_CAMMODE
	{
		S2M_STATE,
		S2M_PART,
	};


private:
	KPtr<Camera> m_Cam;
	KVector2 PrevMouse;
	KVector2 RotPos;
	float m_RotSpeed;
	float m_Speed;
	float m_Boost;


	SC2_CAMMODE m_CMode;

	KVector2 OutSize;
	KVector2 OutPos;

	KPtr<KMesh> m_Mesh;
	KPtr<KMaterial> m_Mtl;
	KPtr<Sampler> m_Smp;



private:
	void Update_State();
	void Update_Part();
	void Update_Key();

	bool Update_ScrCheck();



public:
	KPtr<Camera> camera()
	{
		return m_Cam;
	}
	bool Init();
	void Update();

	void Set_Part()
	{
		m_CMode = SC2_CAMMODE::S2M_PART;
	}
	void Set_State()
	{
		m_CMode = SC2_CAMMODE::S2M_STATE;
	}


	void out_size(const KVector2& _Value)
	{
		OutSize = _Value;
	}

	void out_pos(const KVector2& _Value)
	{
		OutPos = _Value;
	}
};

