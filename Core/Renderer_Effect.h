#pragma once
#include "Renderer.h"
#include <vector>


class Renderer_Effect : public Renderer
{
public:
	class Effect_Begin
	{
		KVector Pos;
		KVector Dir;
		KVector2 Size;
		float LiveTime;
		KVector Type;
	};

public:
	std::vector<Effect_Begin> m_EFVec;
	// StreamBuffer

private:
	bool Init(const KUINT& _DotCnt, const int& _Order = 0);
	void Update() override;
	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data);

public:
	Renderer_Effect();
	~Renderer_Effect();
};

