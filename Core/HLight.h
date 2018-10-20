#pragma once
#include "HBaseCom.h"
#include <list>
#include <set>

class HCamera;
class HMesh;
class HMaterial;
class HLight : public HBaseCom
{
public:
	friend class HRenderMgr;

	enum LIGHTTYPE
	{
		DIR,
		POINT,
		SPOT,
	};

	class LightColor 
	{
	public:
		KVector4 Diff; // 확산광
		KVector4 Spec; // 반사광
		KVector4 Ambi; // 환경광
	};

	class LightData 
	{
	public:
		LightColor Lc;
		KVector4 LightPos;
		KVector4 LightDir;
		KVector4 LightDirInv;
		float Range;
		float Angle;
		int Type;
		int Dump;
	};

	class LightCBDATA 
	{
	public:
		LightData ArrLight[10];
		int LightCount;
		int LightCount1;
		int LightCount2;
		int LightCount3;
	};

private:
	LIGHTTYPE m_eType;
	LightData Data;
	std::set<int> m_RenderGroup;

public:
	MatrixContainer		m_MatData;
	KPtr<HMesh> m_LightMesh;
	KPtr<HMesh> m_VolumMesh;
	KPtr<HMaterial> m_VolumeMat;
	KPtr<HMaterial> m_LightMat;

	// 나중에 여러분들이 필터를 넣을것이라면
	// 필터도 또 따로 세팅을 해줘야 할거다.
public:
	virtual bool Init();
	void EndUpdate() override;

public:
	bool IsLight(int _GroupOrder) const
	{
		std::set<int>::iterator FindIter = m_RenderGroup.find(_GroupOrder);

		if (FindIter == m_RenderGroup.end())
		{
			return false;
		}

		return true;
	}

public:
	template<typename ...Rest>
	void PushLightLayer(Rest ...Arg)
	{
		LightLayerPush(Arg...);
	}

public:
	template<typename ...Rest>
	void LightLayerPush(int _Data, Rest ...Arg)
	{
		std::set<int>::iterator FindIter = m_RenderGroup.find(_Data);

		if (FindIter == m_RenderGroup.end())
		{
			m_RenderGroup.insert(_Data);
		}
		LightLayerPush(Arg...);
	}

	void LightLayerPush() {	}
	void SetType(LIGHTTYPE _Type);

private: ////////// LightRender
	void LightRender(KPtr<HCamera> _Camera);
	void CalLightData(KPtr<HCamera> _Camera);


public:
	HLight();
	~HLight();
};

