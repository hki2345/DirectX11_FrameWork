#pragma once
#include "Renderer.h"
#include "KStreamBuffer.h"
#include <vector>


class Texture;
class Renderer_Effect : public Renderer
{
public:
	class Effect_Begin
	{
	public:
		KVector Pos;
		KUINT Switch;
	};

private:
	// StreamBuffer
	std::vector<Effect_Begin> m_EFVec;
	std::vector<KVector> m_EFPVec;
	KPtr<Texture> m_Tex;
	KPtr<KStreamBuffer> m_InitBuf;
	KPtr<KStreamBuffer> m_SOBuffer;

	KPtr<KMaterial> m_SMtl;
	KPtr<KMaterial> m_RMtl;

private:
	bool Init(const KUINT& _DotCnt, const int& _Order = 0);
	void Update() override;
	void Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data);

public:
	Renderer_Effect();
	~Renderer_Effect();
};

