#pragma once
#include "Renderer.h"
class Renderer_Grid3D :
	public Renderer
{
public:
	Renderer_Grid3D();
	~Renderer_Grid3D();

public:
	struct GRID_DATA
	{
		// float m_GridSize;
		// float m_Border;
		// float m_Alpha;
		KVector4	m_GD;
		KColor		m_Color;
	};


private:
	float m_Border;
	float m_Size;
	GRID_DATA	m_GD;


public:
	void color(const KColor& _Value)
	{
		m_GD.m_Color = _Value;
	}
	void size(const float& _Value)
	{
		m_Size = _Value;
	}

	void Render(KPtr<Camera> _Camera) override;
	void Update_Trans(KPtr<Camera> _Camera) override;
	bool Init(int _Order = 0) override;

private:
	void Set_Data(KPtr<Camera> _Camera);
};

