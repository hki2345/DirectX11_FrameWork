#pragma once
#include "Renderer.h"


class Renderer_UI : public Renderer
{
public:
	Renderer_UI();
	~Renderer_UI();

private:
	KVector m_UISize;

public:
	void UISize(const KVector& _Value)
	{
		m_UISize = _Value;
	}


	void Update_Trans(KPtr<Camera> _Cam) override;
	void Render(KPtr<Camera> _Camera) override;
	bool Init(int _Order = 0) override;
};

