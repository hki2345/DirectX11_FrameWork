#pragma once
#include "Component.h"
#include <set>

class KLight : public Component
{
public:
	friend class RenderManager;

	enum LightType
	{
		Direction = 0,
		Point,
		Spot,
	};

	class LightColor
	{
	public:
		KColor	Color;
		KVector Diff; // 확산광
		KVector Spec; // 반사광
		KVector Ambi; // 환경광
	};

	class LightData
	{
	public:
		LightColor LC;
		KVector m_Pos;
		KVector m_Dir;
		KVector CamPos;
		float Range;
		float Angle;
		int Type;
		int Dump;
	};

	class LightCB
	{
	public:
		LightData ArrLight[10];
		int LCnt;

		// 16바이트 ㅡ.ㅡ;;
		int Temp1;
		int Temp2;
		int Temp3;
	};

private:
	int m_Order;
	std::set<int> m_LayerSet;
	LightData m_LD;

public:
	void type(const LightType& _type)
	{
		m_LD.Type = (int)_type;
	}

	void range(const float& _Value)
	{
		m_LD.Range = _Value;
	}

	void color(const KColor& _Value)
	{
		m_LD.LC.Color = _Value;
	}



	virtual bool Init();
	void FinalUpdate() override;


	bool Is_Focus(const int& _Layer) const
	{
		std::set<int>::iterator FI = m_LayerSet.find(_Layer);

		if (m_LayerSet.end() == FI)
		{
			return false;
		}

		return true;
	}


private:
	// 나머지를 넣는 과정
	template<typename ...Rest>
	void Insert_LayerTemp(Rest ...Arg)
	{
		Insert_Layer(Arg...);
	}

public:
	template<typename ...Rest>
	void Insert_Layer(int _Data, Rest ...Arg)
	{
		std::set<int>::iterator FI = m_LayerSet.find(_Data);

		if (m_LayerSet.end() == FI)
		{
			m_LayerSet.insert(_Data);
		}
		Insert_Layer(Arg...);
	}

	void Insert_Layer() {}

public:
	KLight();
	~KLight();
};

