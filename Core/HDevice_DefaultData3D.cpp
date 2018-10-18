#include "HDevice.h"
#include "DVHeader.h"
#include "HWindow.h"
#include "Stl_AID.h"
#include "HMesh.h"
#include "HVAR.h"
#include "HVtxShader.h"
#include "HPixShader.h"
#include "HResMgr.h"
#include "HMaterial.h"
#include "HBlend.h"
#include "HFont.h"
#include "GameMath.h"
#include "HLight.h"
#include "HRenderer.h"
#include "HTexture.h"
#include "HSampler.h"
#include "HMultiRenderTaget.h"



bool HDevice::DefRenderTaget() 
{


	// Defferd용
	HResMgr<HRenderTaget>::Create(L"COLOR_DIFFUSE" , HVAR::MainWindow().UWidth() , HVAR::MainWindow().UHeight()		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE	, DXGI_FORMAT_R32G32B32A32_FLOAT);
	HResMgr<HRenderTaget>::Create(L"POSTION", HVAR::MainWindow().UWidth(), HVAR::MainWindow().UHeight(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	HResMgr<HRenderTaget>::Create(L"NORMAL", HVAR::MainWindow().UWidth(), HVAR::MainWindow().UHeight(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	HResMgr<HRenderTaget>::Create(L"DEPTH", HVAR::MainWindow().UWidth(), HVAR::MainWindow().UHeight(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	KPtr<HMultiRenderTaget> DefMRT = HResMgr<HMultiRenderTaget>::Create(L"DEFFERD", L"COLOR_DIFFUSE", L"POSTION", L"NORMAL", L"DEPTH");

	HResMgr<HRenderTaget>::Create(L"LIGHT_DIFFUSE", HVAR::MainWindow().UWidth(), HVAR::MainWindow().UHeight(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	HResMgr<HRenderTaget>::Create(L"LIGHT_SPECULAR", HVAR::MainWindow().UWidth(), HVAR::MainWindow().UHeight(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	KPtr<HMultiRenderTaget> LightMRT = HResMgr<HMultiRenderTaget>::Create(L"LIGHT", L"LIGHT_DIFFUSE", L"LIGHT_SPECULAR");

	return true;
}

bool HDevice::Def3DCreate() 
{
	HResMgr<HTexture>::Load(L"Texture", L"Sky01.png");

	HResMgr<HSampler>::Create(L"DefaultSmp");
	HResMgr<HBlend>::Create(L"Alpha");
	HResMgr<HFont>::Create(L"궁서", L"궁서");

	HVAR::MainDevice().CreateCB<MATDATA>(L"MATDATA", D3D11_USAGE_DYNAMIC, 10);
	HVAR::MainDevice().CreateCB<RenderOption>(L"RENDEROPTION", D3D11_USAGE_DYNAMIC, 11);
	HVAR::MainDevice().CreateCB<HLight::LightCBDATA>(L"LIGHTDATA", D3D11_USAGE_DYNAMIC, 12);
	HVAR::MainDevice().CreateCB<HLight::LightData>(L"DEFFERDLIGHTDATA", D3D11_USAGE_DYNAMIC, 12);

	HVAR::MainDevice().CreateRsMode(L"SNONE", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);
	HVAR::MainDevice().CreateRsMode(L"SBACK", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK);
	HVAR::MainDevice().CreateRsMode(L"SFRONT", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_FRONT);

	HVAR::MainDevice().CreateRsMode(L"WNONE", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_NONE);
	HVAR::MainDevice().CreateRsMode(L"WBACK", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_BACK);
	HVAR::MainDevice().CreateRsMode(L"WFRONT", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_FRONT);
	HVAR::MainDevice().SetDefRsMode(L"SBACK");

	// 뎊스를 켜놓고
	D3D11_DEPTH_STENCIL_DESC DepthState;

	DepthState.DepthEnable = TRUE;
	// D3D11_DEPTH_WRITE_MASK_ALL 뎊스비교를 하겠다는 것이다.
	// D3D11_DEPTH_WRITE_MASK_ZERO 쓰지 않겠다.
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	// 스텐실에 관련된 것.
	// 0x000000ff 가장 뒤에 사용하겠다.
	DepthState.StencilEnable = FALSE;
	DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	D3D11_DEPTH_STENCILOP_DESC defaultStencil =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	DepthState.FrontFace = defaultStencil;
	DepthState.BackFace = defaultStencil;
	HVAR::MainDevice().CreateDsMode(L"DEBUG", DepthState);

	// 정상적인것.
	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	HVAR::MainDevice().CreateDsMode(L"BASIC", DepthState);

	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	HVAR::MainDevice().CreateDsMode(L"ALWAYS", DepthState);

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	HVAR::MainDevice().CreateDsMode(L"LIGHTDEPTH", DepthState);


	//////////////// VOLUMBACK
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_GREATER;
	DepthState.StencilEnable = TRUE;

	D3D11_DEPTH_STENCILOP_DESC LightStencil;
	LightStencil.StencilFunc = D3D11_COMPARISON_ALWAYS;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;
	HVAR::MainDevice().CreateDsMode(L"BACK_ST", DepthState);

	//////////////// VOLUMFORONT
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	DepthState.StencilEnable = TRUE;

	LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;
	HVAR::MainDevice().CreateDsMode(L"FRONT_ST", DepthState);

	//////////////// PASS

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	DepthState.StencilEnable = TRUE;

	LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_ZERO;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;

	HVAR::MainDevice().CreateDsMode(L"PASS_ST", DepthState);

	// 하나를 더 만들어야 한다.

	HVAR::MainDevice().SetDefDsMode(L"BASIC");



	///////////////////////////////////////////// BS
	D3D11_BLEND_DESC BDesc;
	BDesc.AlphaToCoverageEnable = false;

	// 다른 랜더 타겟도 따로따로 쓰겠다 혹은 아니다.
	// 블랜드 설정이 false로 하면 0번으로 초기화 된다.
	BDesc.IndependentBlendEnable = true;


	// 디퓨즈 -> 디퓨즈는 기존 꺼에 덮어 씌우는 개념 ㅇㅇ
	BDesc.RenderTarget[0].BlendEnable = true;
	BDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	BDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	BDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;


	// 스펙 큘러 - 색이 있으면 알파 랜더러 적요 -> 기존 알파렌더와 같은 방식으로 적용된다.
	// 기존 알파 방식은 -> 알파와 알파가 섞이는 식임
	BDesc.RenderTarget[1].BlendEnable = true;
	BDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	BDesc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	BDesc.RenderTarget[1].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BDesc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	BDesc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BDesc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	BDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;


	for (size_t i = 2; i < 8; i++)
	{
		BDesc.RenderTarget[i] = D3D11_RENDER_TARGET_BLEND_DESC{};
	}

	HResMgr<HBlend>::Create(L"LIGHTONE", BDesc);

	BDesc.RenderTarget[0] = D3D11_RENDER_TARGET_BLEND_DESC{};
	BDesc.RenderTarget[0].RenderTargetWriteMask = 0;


	HResMgr<HBlend>::Create(L"VOLUME", BDesc);

	return true;
}
bool HDevice::Mesh3DCreate() {

	////////////////////////////////// RECT
#pragma region RectMeshCreate
	VTX3D ArrVTX3D[4];
	ArrVTX3D[0].Pos = HVEC(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrVTX3D[1].Pos = HVEC(0.5f, 0.5f, 0.0f, 1.0f);
	ArrVTX3D[2].Pos = HVEC(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrVTX3D[3].Pos = HVEC(0.5f, -0.5f, 0.0f, 1.0f);

	ArrVTX3D[0].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[1].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[2].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[3].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);

	ArrVTX3D[0].Uv = HVEC2(0.0f, 0.0f);
	ArrVTX3D[1].Uv = HVEC2(1.0f, 0.0f);
	ArrVTX3D[2].Uv = HVEC2(0.0f, 1.0f);
	ArrVTX3D[3].Uv = HVEC2(1.0f, 1.0f);

	ArrVTX3D[0].Normal = HVEC(0.0f, 0.0f, -1.0f, 1.0f);
	ArrVTX3D[1].Normal = HVEC(0.0f, 0.0f, -1.0f, 1.0f);
	ArrVTX3D[2].Normal = HVEC(0.0f, 0.0f, -1.0f, 1.0f);
	ArrVTX3D[3].Normal = HVEC(0.0f, 0.0f, -1.0f, 1.0f);

	IDX16 ArrColorIDX[2] = {};

	ArrColorIDX[0] = IDX16(0, 3, 2);
	ArrColorIDX[1] = IDX16(0, 1, 3);

	HResMgr<HMesh>::Create(L"RECT"
		, 4, (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, ArrVTX3D
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrColorIDX
		, IDX16::FM());
#pragma endregion

#pragma region BoxMeshCreate

	// 정육면체 매쉬부터 만들어 보자.
	std::vector<VTX3D> OverCube;
	std::vector<WORD> OverIdx;

	VTX3D V;
	V.Pos = HVEC{ -0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ -0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	// 아랫면
	V.Pos = HVEC{ -0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ -0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);


	// 오른쪽
	V.Pos = HVEC{ 0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	// 왼쪽
	V.Pos = HVEC{ -0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ -0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ -0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ -0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	// 앞
	V.Pos = HVEC{ -0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ -0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	// 뒤
	V.Pos = HVEC{ -0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ 0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = HVEC{ -0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = HVEC2{ 0.0f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = HVEC{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	// 윗면 인덱스
	OverIdx.push_back(0); OverIdx.push_back(1); OverIdx.push_back(2);
	OverIdx.push_back(0); OverIdx.push_back(2); OverIdx.push_back(3);

	// 아랫면
	OverIdx.push_back(6); OverIdx.push_back(5); OverIdx.push_back(4);
	OverIdx.push_back(6); OverIdx.push_back(4); OverIdx.push_back(7);

	// 오른쪽
	OverIdx.push_back(8); OverIdx.push_back(9); OverIdx.push_back(10);
	OverIdx.push_back(8); OverIdx.push_back(10); OverIdx.push_back(11);

	// 왼쪽 면
	OverIdx.push_back(13); OverIdx.push_back(12); OverIdx.push_back(15);
	OverIdx.push_back(13); OverIdx.push_back(15); OverIdx.push_back(14);

	// 앞면
	OverIdx.push_back(17); OverIdx.push_back(16); OverIdx.push_back(19);
	OverIdx.push_back(17); OverIdx.push_back(19); OverIdx.push_back(18);

	// 뒷면
	OverIdx.push_back(20); OverIdx.push_back(21); OverIdx.push_back(22);
	OverIdx.push_back(20); OverIdx.push_back(22); OverIdx.push_back(23);

	HResMgr<HMesh>::Create(L"CUBE"
		, (UINT)OverCube.size(), (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, &OverCube[0]
		, (UINT)OverIdx.size(), (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, &OverIdx[0]
		, IDX16::FM());

#pragma endregion

#pragma region SphereMeshCreate
	std::vector<VTX3D> SphereVtx;
	std::vector<UINT> SphereIdx;

	float fRadius = 0.5f;

	// 북극점
	V.Pos = HVEC{ 0.0f, fRadius, 0.0f, 1.0f };
	V.Uv = HVEC2{ 0.5f, 0.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = V.Pos;
	V.Normal.Normalize3D();
	V.Normal.w = 0.0f;
	V.Tangent = HVEC{ 1.0f, 0.0f, 0.0f, 0.0f };
	V.BNormal = HVEC{ 0.0f, 0.0f, 1.0f, 0.0f };

	SphereVtx.push_back(V);

	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float yRotAngle = DirectX::XM_PI / (float)iStackCount;
	float zRotAngle = DirectX::XM_2PI / (float)iSliceCount;

	float yUvRatio = 1.0f / (float)iSliceCount;
	float zUvRatio = 1.0f / (float)iStackCount;

	// PointToDir

	for (UINT y = 1; y < iStackCount; ++y)
	{
		float phi = y * yRotAngle;

		for (UINT z = 0; z < iSliceCount + 1; ++z)
		{
			float theta = z * zRotAngle;

			V.Pos = HVEC
			{
				fRadius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
				fRadius * cosf(y * yRotAngle),
				fRadius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
				1.0f
			};

			V.Pos.EpCut();

			V.Pos.w = 1.0f;
			V.Uv = HVEC2{ yUvRatio * z, zUvRatio * y };
			V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
			V.Normal = V.Pos;
			V.Normal.Normalize3D();
			V.Normal.w = 0.0f;

			V.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
			V.Tangent.y = 0.0f;
			V.Tangent.z = fRadius * sinf(phi) * cosf(theta);
			V.Tangent.Normalize3D();
			V.Tangent.w = 0.0f;

			V.BNormal = HVEC::Cross3D(V.Tangent, V.Normal);
			V.BNormal.Normalize3D();
			V.BNormal.w = 0.0f;

			SphereVtx.push_back(V);
		}
	}

	// 북극점
	V.Pos = HVEC{ 0.0f, -fRadius, 0.0f, 1.0f };
	V.Uv = HVEC2{ 0.5f, 1.0f };
	V.Color = HVEC{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = V.Pos;
	V.Normal.Normalize3D();
	V.Normal.w = 0.0f;

	SphereVtx.push_back(V);

	SphereIdx.clear();

	// 북극점 인덱스
	for (UINT i = 0; i < iSliceCount; i++)
	{
		SphereIdx.push_back(0);
		SphereIdx.push_back(i + 2);
		SphereIdx.push_back(i + 1);
	}


	for (UINT y = 0; y < iStackCount - 2; ++y)
	{
		for (UINT z = 0; z < iSliceCount; ++z)
		{
			SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + z + 1);

			SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
			SphereIdx.push_back((iSliceCount + 1) * y + (z + 1) + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
		}
	}

	// 북극점 인덱스

	UINT iBotStart = (UINT)SphereVtx.size() - 1;

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		SphereIdx.push_back(iBotStart);
		SphereIdx.push_back(iBotStart - (i + 2));
		SphereIdx.push_back(iBotStart - (i + 1));
	}

	HResMgr<HMesh>::Create(L"SPHERE"
		, (UINT)SphereVtx.size(), (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, &SphereVtx[0]
		, (UINT)SphereIdx.size(), (UINT)IDX32::MemberSize(), D3D11_USAGE_DEFAULT, &SphereIdx[0]
		, IDX32::FM());

#pragma endregion
	return true;
}
bool HDevice::Mat3DCreate() {

	// NONE
	KPtr<HVtxShader> NONE3DVTX = HResMgr<HVtxShader>::LoadToKey(L"NONEVTX", L"Shader", L"NONE.fx", "VTXNONE3D");
	NONE3DVTX->AddLayoutEnd("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<HPixShader> NONE3DPIX = HResMgr<HPixShader>::LoadToKey(L"NONEPIX", L"Shader", L"NONE.fx", "PIXNONE3D");

	KPtr<HMaterial> NONEMAT = HResMgr<HMaterial>::Create(L"NONEMAT");
	NONEMAT->SetVtxShader(L"NONEVTX");
	NONEMAT->SetPixShader(L"NONEPIX");
	NONEMAT->SetBlend(L"Alpha");

	KPtr<HVtxShader> TAGETDEBUGVTX = HResMgr<HVtxShader>::LoadToKey(L"TAGETDEBUGVTX", L"Shader", L"TagetDebug.fx", "VS_TagetTex");
	TAGETDEBUGVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TAGETDEBUGVTX->AddLayoutEnd("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<HPixShader> TAGETDEBUGPIX = HResMgr<HPixShader>::LoadToKey(L"TAGETDEBUGPIX", L"Shader", L"TagetDebug.fx", "PS_TagetTex");

	KPtr<HMaterial> TAGETDEBUGMAT = HResMgr<HMaterial>::Create(L"TAGETDEBUGMAT");
	TAGETDEBUGMAT->SetVtxShader(L"TAGETDEBUGVTX");
	TAGETDEBUGMAT->SetPixShader(L"TAGETDEBUGPIX");
	TAGETDEBUGMAT->SetBlend(L"Alpha");

	///////////////////////////////////////// GRID
	KPtr<HVtxShader> GRID3DVTX = HResMgr<HVtxShader>::LoadToKey(L"GRID3DVTX", L"Shader", L"GRID3D.fx", "VS_GRID3D");
	GRID3DVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	GRID3DVTX->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	GRID3DVTX->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	GRID3DVTX->AddLayoutEnd("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	KPtr<HPixShader> GRID3DPIX = HResMgr<HPixShader>::LoadToKey(L"GRID3DPIX", L"Shader", L"GRID3D.fx", "PS_GRID3D");
	GRID3DPIX->CreateCB<HVEC>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 0);

	KPtr<HMaterial> GRID3DMAT = HResMgr<HMaterial>::Create(L"GRID3DMAT");
	GRID3DMAT->SetVtxShader(L"GRID3DVTX");
	GRID3DMAT->SetPixShader(L"GRID3DPIX");
	GRID3DMAT->SetBlend(L"Alpha");

	// RECT
	KPtr<HVtxShader> RECT3DVTX = HResMgr<HVtxShader>::LoadToKey(L"RECT3DVTX", L"Shader", L"RECT3D.fx", "VS_RECT3D");
	RECT3DVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RECT3DVTX->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	RECT3DVTX->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RECT3DVTX->AddLayoutEnd("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<HPixShader> RECT3DPIX = HResMgr<HPixShader>::LoadToKey(L"RECT3DPIX", L"Shader", L"RECT3D.fx", "PS_RECT3D");

	KPtr<HMaterial> RECT3DMAT = HResMgr<HMaterial>::Create(L"RECT3DMAT");
	RECT3DMAT->SetVtxShader(L"RECT3DVTX");
	RECT3DMAT->SetPixShader(L"RECT3DPIX");
	RECT3DMAT->SetBlend(L"Alpha");

	// SKY
	KPtr<HVtxShader> SKY3DVTX = HResMgr<HVtxShader>::LoadToKey(L"SKY3DVTX", L"Shader", L"SkyBox.fx", "VS_SKY3D");
	SKY3DVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SKY3DVTX->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	SKY3DVTX->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SKY3DVTX->AddLayoutEnd("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<HPixShader> SKY3DPIX = HResMgr<HPixShader>::LoadToKey(L"SKY3DPIX", L"Shader", L"SkyBox.fx", "PS_SKY3D");

	KPtr<HMaterial> SKY3DMAT = HResMgr<HMaterial>::Create(L"SKY3DMAT");
	SKY3DMAT->SetVtxShader(L"SKY3DVTX");
	SKY3DMAT->SetPixShader(L"SKY3DPIX");
	SKY3DMAT->SetBlend(L"Alpha");
	SKY3DMAT->AddTexData(TEXTYPE::TT_COLOR, 0, L"Sky01.png");

	KPtr<HVtxShader> MESH3DVTX = HResMgr<HVtxShader>::LoadToKey(L"MESH3DVTX", L"Shader", L"MeshMat.fx", "VS_MESH3D");
	MESH3DVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	MESH3DVTX->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->AddLayout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->AddLayout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->AddLayoutEnd("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<HPixShader> MESH3DPIX = HResMgr<HPixShader>::LoadToKey(L"MESH3DPIX", L"Shader", L"MeshMat.fx", "PS_MESH3D");

	KPtr<HMaterial> MESH3DMAT = HResMgr<HMaterial>::Create(L"MESH3DMAT");
	MESH3DMAT->SetVtxShader(L"MESH3DVTX");
	MESH3DMAT->SetPixShader(L"MESH3DPIX");
	MESH3DMAT->SetBlend(L"Alpha");

	KPtr<HVtxShader> DEFFERD3DVTX = HResMgr<HVtxShader>::LoadToKey(L"DEFFERD3DVTX", L"Shader", L"Defferd.fx", "VS_DEFFERD");
	DEFFERD3DVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	DEFFERD3DVTX->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->AddLayout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->AddLayout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->AddLayoutEnd("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<HPixShader> DEFFERD3DPIX = HResMgr<HPixShader>::LoadToKey(L"DEFFERD3DPIX", L"Shader", L"Defferd.fx", "PS_DEFFERD");

	KPtr<HMaterial> DEFFERD3DMAT = HResMgr<HMaterial>::Create(L"DEFFERD3DMAT");
	DEFFERD3DMAT->SetVtxShader(L"DEFFERD3DVTX");
	DEFFERD3DMAT->SetPixShader(L"DEFFERD3DPIX");
	DEFFERD3DMAT->SetBlend(L"Alpha");

	// DefferdDirLight
	KPtr<HVtxShader> DEFFERDLIGHTVTX = HResMgr<HVtxShader>::LoadToKey(L"DEFFERDLIGHTVTX", L"Shader", L"Defferd.fx", "VS_DEFFERDLIGHT");
	DEFFERDLIGHTVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDLIGHTVTX->AddLayoutEnd("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<HPixShader> DEFFERDLIGHTPIX = HResMgr<HPixShader>::LoadToKey(L"DEFFERDLIGHTPIX", L"Shader", L"Defferd.fx", "PS_DEFFERDLIGHT");

	KPtr<HMaterial> DEFFERDMAT = HResMgr<HMaterial>::Create(L"DEFFERDLIGHTMAT");
	DEFFERDMAT->SetVtxShader(L"DEFFERDLIGHTVTX");
	DEFFERDMAT->SetPixShader(L"DEFFERDLIGHTPIX");
	DEFFERDMAT->SetBlend(L"LIGHTONE");
	DEFFERDMAT->AddTexData(TEXTYPE::TT_TAGET, 0, L"POSTION");
	DEFFERDMAT->AddTexData(TEXTYPE::TT_TAGET, 1, L"NORMAL");
	DEFFERDMAT->AddTexData(TEXTYPE::TT_TAGET, 2, L"DEPTH");

	// MERGE
	KPtr<HVtxShader> DEFFERDMERGEVTX = HResMgr<HVtxShader>::LoadToKey(L"DEFFERDMERGEVTX", L"Shader", L"Defferd.fx", "VS_DEFFERDMERGE");
	DEFFERDMERGEVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDMERGEVTX->AddLayoutEnd("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<HPixShader> DEFFERDMERGEPIX = HResMgr<HPixShader>::LoadToKey(L"DEFFERDMERGEPIX", L"Shader", L"Defferd.fx", "PS_DEFFERDMERGE");

	KPtr<HMaterial> DEFFERDMERGEMAT = HResMgr<HMaterial>::Create(L"DEFFERDMERGEMAT");
	DEFFERDMERGEMAT->SetVtxShader(L"DEFFERDMERGEVTX");
	DEFFERDMERGEMAT->SetPixShader(L"DEFFERDMERGEPIX");
	DEFFERDMERGEMAT->SetBlend(L"Alpha");
	DEFFERDMERGEMAT->AddTexData(TEXTYPE::TT_TAGET, 0, L"COLOR_DIFFUSE");
	DEFFERDMERGEMAT->AddTexData(TEXTYPE::TT_TAGET, 1, L"LIGHT_DIFFUSE");
	DEFFERDMERGEMAT->AddTexData(TEXTYPE::TT_TAGET, 2, L"LIGHT_SPECULAR");

	KPtr<HVtxShader> SCREENMERGEVTX = HResMgr<HVtxShader>::LoadToKey(L"SCREENMERGEVTX", L"Shader", L"ScreenMerge.fx", "VS_SCREENMERGE");
	SCREENMERGEVTX->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SCREENMERGEVTX->AddLayoutEnd("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<HPixShader> SCREENMERGEPIX = HResMgr<HPixShader>::LoadToKey(L"SCREENMERGEPIX", L"Shader", L"ScreenMerge.fx", "PS_SCREENMERGE");

	KPtr<HMaterial> SCREENMERGEMAT = HResMgr<HMaterial>::Create(L"SCREENMERGEMAT");
	SCREENMERGEMAT->SetVtxShader(L"SCREENMERGEVTX");
	SCREENMERGEMAT->SetPixShader(L"SCREENMERGEPIX");

	KPtr<HVtxShader> VOLUMEVTX = HResMgr<HVtxShader>::LoadToKey(L"VOLUMEVTX", L"Shader", L"VolumeMesh.fx", "VS_VOLUME");
	VOLUMEVTX->AddLayoutEnd("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<HPixShader> VOLUMEPIX = HResMgr<HPixShader>::LoadToKey(L"VOLUMEPIX", L"Shader", L"VolumeMesh.fx", "PS_VOLUME");

	KPtr<HMaterial> VOLUMEMAT = HResMgr<HMaterial>::Create(L"VOLUMEMAT");
	DEFFERDMAT->SetBlend(L"VOLUME");
	VOLUMEMAT->SetVtxShader(L"VOLUMEVTX");
	VOLUMEMAT->SetPixShader(L"VOLUMEPIX");

	return true;
}

bool HDevice::DefaultDataInit3D() 
{

	DefRenderTaget();
	Def3DCreate();
	Mesh3DCreate();
	Mat3DCreate();

	return true;
}
