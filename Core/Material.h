#pragma once
#include "KResource.h"
#include "Vertex_Shader.h"
#include "Pixel_Shader.h"
#include "Blend.h"


class Material : public KResource
{
public:
	KPtr<Vertex_Shader> m_Vertex_Shader;
	KPtr<Pixel_Shader> m_Pixel_Shader;
	KPtr<Blend>			m_Blend;

public:
	KPtr<Vertex_Shader> VShader()
	{
		return m_Vertex_Shader;
	}
	KPtr<Pixel_Shader> PShader()
	{
		return m_Pixel_Shader;
	}

	bool Set_VShader(const wchar_t* _Name);
	bool Set_PShader(const wchar_t* _Name);
	bool Set_Blend(const wchar_t* _Name);


	bool Create();
	void Update();

public:
	Material();
	~Material();
};

