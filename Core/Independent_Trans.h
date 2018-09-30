#pragma once
#include "Component_DE.h"

// �θ� ���ӵǾ��־ ���������� �ൿ�� �� �ְԲ� ����
class Independent_Trans : public Component_DE
{
private:
	enum INDEPEN_MODE
	{
		BASE,
		CONTROLLED,
		IDEPENDENT,
	};

private:
	INDEPEN_MODE m_eMode;
	KVector4 m_Scale;
	KVector4 m_Rotate;

	// ��ġ�� ������ �θ� ������ �Ǿ�����
	KVector4 m_Pivot;

	KMatrix m_ScaleMat;
	KMatrix m_RotateMat;
	KMatrix m_PosMat;

	KMatrix m_RMatX, m_RMatY, m_RMatZ; // ������ �ƴ� ����
	KMatrix m_IndenMat; // �������� ��Ʈ���� -> �̰��� �����Ѵ�.

public:
	void Init_Component() override;
	
	
	void Base_Mode() { m_eMode = INDEPEN_MODE::BASE; }
	void Controlled_Mode()  { m_eMode = INDEPEN_MODE::CONTROLLED; }
	void Independent_Mode()  { m_eMode = INDEPEN_MODE::IDEPENDENT; }

	void inde_scale(const KVector4& _Value)
	{
		m_Scale = _Value;
		m_Trans->BLocalScale(true);
		m_Trans->BWorld(true);
		Independent_Mode();
	}
	void inde_rotation(const KVector4& _Value)
	{
		m_Rotate = _Value;
		m_Trans->BLocalRotate(true);
		m_Trans->BWorld(true);
		Independent_Mode();
	}
	void inde_pivot(const KVector4& _Value) 
	{
		m_Pivot = _Value;
		m_Trans->BLocalPostion(true);
		m_Trans->BWorld(true);
		Independent_Mode();
	}

	KVector4& inde_scale()
	{
		return m_Scale;
	}
	KVector4& inde_rotation()
	{
		return m_Rotate;
	}
	KVector4& inde_pivot()
	{
		return m_Pivot;
	}

	KVector4 pivot_world() { return m_IndenMat.vec4; }

	const KMatrix& independent_Matrix() const { return m_IndenMat; }
	void Indepen_Update();

public:
	Independent_Trans();
	~Independent_Trans();
};

