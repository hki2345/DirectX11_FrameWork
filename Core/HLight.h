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
		HVEC Diff; // Ȯ�걤
		HVEC Spec; // �ݻ籤
		HVEC Ambi; // ȯ�汤
	};

	class LightData 
	{
	public:
		LightColor Lc;
		HVEC LightPos;
		HVEC LightDir;
		HVEC LightDirInv;
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
	MATDATA		m_MatData;
	HPTR<HMesh> m_LightMesh;
	HPTR<HMesh> m_VolumMesh;
	HPTR<HMaterial> m_VolumeMat;
	HPTR<HMaterial> m_LightMat;

	// ���߿� �����е��� ���͸� �������̶��
	// ���͵� �� ���� ������ ����� �ҰŴ�.
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
	void LightRender(HPTR<HCamera> _Camera);
	void CalLightData(HPTR<HCamera> _Camera);


public:
	HLight();
	~HLight();
};

