#include "KDevice.h"
#include "KWindow.h"
#include "Core_Class.h"
#include "Pixel_Shader.h"
#include "Stl_Assistor.h"

#include "DirectXHeader_DE.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Blend.h"

#include "KMath.h"
#include "KFont.h"


#define CIRCLE_NUM 360



bool KDevice::Init_DefaultData_2D()
{
	ResourceManager<Blend>::Create(L"AlphaBlend");
	/************************* Material _ Line **************************/
	Vertex_Texture TextureVert[4] = {};
	Index_16 TextureIdx[2] = {};

	TextureVert[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	TextureVert[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	TextureVert[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	TextureVert[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	TextureVert[0].Uv = KVector2(0.0f, 0.0f);
	TextureVert[1].Uv = KVector2(1.0f, 0.0f);
	TextureVert[2].Uv = KVector2(0.0f, 1.0f);
	TextureVert[3].Uv = KVector2(1.0f, 1.0f);

	TextureIdx[0] = Index_16(0, 3, 2);
	TextureIdx[1] = Index_16(0, 1, 3);

	ResourceManager<Mesh>::Create(L"LINEMESH",
		4, (UINT)sizeof(Vertex_Texture), D3D11_USAGE_DYNAMIC, TextureVert,
		6, (UINT)Index_16::MemberSize(), D3D11_USAGE_DEFAULT, TextureIdx,
		Index_16::FM());




	// 원
	KVector4 CLineVertex[CIRCLE_NUM + 1] = {};
	Index_16 Circle_Index[CIRCLE_NUM];

	// 버텍스 설정
	for (size_t i = 0; i < CIRCLE_NUM; ++i)
	{
		CLineVertex[i] = KVector(cos(i * KMath::DegToRad * -1.0f), sin(i * KMath::DegToRad* -1.0f), .0f, 1.0f);
	}

	// 중심점 버텍스 - 색을 채울 때 중심을 기준으로 잇기 위해서
	CLineVertex[CIRCLE_NUM] = KVector(.0f, .0f, .0f, 1.0f);

	// 색을 채우는 단계
	for (WORD i = 0; i < CIRCLE_NUM - 1; ++i)
	{
		Circle_Index[i] = Index_16(i, i+1, CIRCLE_NUM);
	}
	// 마지막 각도 색 채우기
	Circle_Index[CIRCLE_NUM - 1] = Index_16(CIRCLE_NUM - 1, 0, CIRCLE_NUM);

	ResourceManager<Mesh>::Create(L"LINECIRCLEMESH",
		CIRCLE_NUM + 1, (UINT)sizeof(KVector4), D3D11_USAGE_DYNAMIC, CLineVertex,
		CIRCLE_NUM * 3, (UINT)Index_16::MemberSize(), D3D11_USAGE_DEFAULT, Circle_Index
		, Index_16::FM()/*, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP*/);




	KPtr<Vertex_Shader> LineVert =
		ResourceManager<Vertex_Shader>::Load_FromKey(L"LINE_VERTEX", L"Shader", L"DebugRectShader.fx", "VS_DebugRect");

	LineVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	LineVert->Add_Layout("UV", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	LineVert->Create_LayOut();

	ResourceManager<Pixel_Shader>::Load_FromKey(L"LINE_PIXEL", L"Shader", L"DebugRectShader.fx", "PS_DebugRect");

	KPtr<Material> LineMaterial = ResourceManager<Material>::Create(L"LINEMAT");
	LineMaterial->Set_VShader(L"LINE_VERTEX");
	LineMaterial->Set_PShader(L"LINE_PIXEL");


	/************************* Material _ Sprite **************************/
	Vertex_Color NewColorVertex[4] = {};
	Index_16 NewColorIndex[2] = {};

	NewColorVertex[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	NewColorVertex[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	NewColorVertex[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	NewColorVertex[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	NewColorVertex[0].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	NewColorVertex[1].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	NewColorVertex[2].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	NewColorVertex[3].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);

	NewColorIndex[0] = Index_16(0, 3, 2);
	NewColorIndex[1] = Index_16(0, 1, 3);



	ResourceManager<Mesh>::Create(L"COLORMESH",
		4, (UINT)Vertex_Color::TypeSize(), D3D11_USAGE_DYNAMIC, NewColorVertex,
		6, (UINT)Index_16::MemberSize(), D3D11_USAGE_DEFAULT, NewColorIndex
		, Index_16::FM());


	KPtr<Vertex_Shader> ColorVert =
		ResourceManager<Vertex_Shader>::Load_FromKey(L"VSCOLOR", L"Shader", L"ColorShader.fx", "VS_Color");

	ColorVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColorVert->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColorVert->Create_LayOut();

	ResourceManager<Pixel_Shader>::Load_FromKey(L"PSCOLOR", L"Shader", L"ColorShader.fx", "PS_Color");

	KPtr<Material> ColorMaterial = ResourceManager<Material>::Create(L"COLORMAT");
	ColorMaterial->Set_VShader(L"VSCOLOR");
	ColorMaterial->Set_PShader(L"PSCOLOR");





	/************************* Texture _ Sprite **************************/
	ResourceManager<Mesh>::Create(L"TEXMESH",
		4, (UINT)Vertex_Texture::TypeSize(), D3D11_USAGE_DYNAMIC, TextureVert,
		6, (UINT)Index_16::MemberSize(), D3D11_USAGE_DEFAULT, TextureIdx
		, Index_16::FM());


	KPtr<Vertex_Shader> TexVert =
		ResourceManager<Vertex_Shader>::Load_FromKey(L"VSTEX", L"Shader", L"TexShader.fx", "VS_Tex");

	TexVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TexVert->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	TexVert->Create_LayOut();

	ResourceManager<Pixel_Shader>::Load_FromKey(L"PSTEX", L"Shader", L"TexShader.fx", "PS_Tex");


	KPtr<Material> TexMaterial = ResourceManager<Material>::Create(L"TEXMAT");
	TexMaterial->Set_VShader(L"VSTEX");
	TexMaterial->Set_PShader(L"PSTEX");
	TexMaterial->Set_Blend(L"AlphaBlend");




	KPtr<Vertex_Shader> NewImage = ResourceManager<Vertex_Shader>::Load_FromKey(L"VSIMG", L"Shader", L"ImageShader.fx", "VS_Img");

	NewImage->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewImage->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	NewImage->Create_LayOut();
	ResourceManager<Pixel_Shader>::Load_FromKey(L"PSIMG", L"Shader", L"ImageShader.fx", "PS_Img");


	KPtr<Material> ImgMaterial = ResourceManager<Material>::Create(L"IMGMAT");
	ImgMaterial->Set_VShader(L"VSIMG");
	ImgMaterial->Set_PShader(L"PSIMG");
	ImgMaterial->Set_Blend(L"AlphaBlend");
	return true;
}



void KDevice::Init_Sprite()
{

}

void KDevice::Init_Circle()
{

}