#include "KDevice.h"
#include "DXContainer_DE.h"
#include "KWindow.h"
#include "Stl_AID.h"
#include "HMesh.h"
#include "Core_Class.h"
#include "HVtxShader.h"
#include "HPixShader.h"
#include "HResMgr.h"
#include "HMaterial.h"
#include "HBlend.h"
#include "HFont.h"

bool KDevice::DefaultDataInit() 
{
	ResourceManager<HBlend>::Create(L"AlphaBlend");
	ResourceManager<HFont>::Create(L"±Ã¼­", L"±Ã¼­");

	// Color Vtx
	COLORVTX ArrColorVTX[4] = {};
	IDX16 ArrColorIDX[2] = {};
	ArrColorVTX[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrColorVTX[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	ArrColorVTX[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrColorVTX[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	ArrColorVTX[0].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[1].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[2].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[3].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);

	ArrColorIDX[0] = IDX16(0, 3, 2);
	ArrColorIDX[1] = IDX16(0, 1, 3);

	WORD ArrLineRectIdX[5] = { 0, 1, 3, 2, 0 };



	// Tex Vtx
	TEXVTX ArrTexVTX[4] = {};
	IDX16 ArrTexIDX[2] = {};
	ArrTexVTX[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVTX[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVTX[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrTexVTX[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	ArrTexVTX[0].Uv = KVector2(0.0f, 0.0f);
	ArrTexVTX[1].Uv = KVector2(1.0f, 0.0f);
	ArrTexVTX[2].Uv = KVector2(0.0f, 1.0f);
	ArrTexVTX[3].Uv = KVector2(1.0f, 1.0f);

	ArrTexIDX[0] = IDX16(0, 3, 2);
	ArrTexIDX[1] = IDX16(0, 1, 3);


	// Boder Rect Shader
	KPtr<HVtxShader> BRectV = ResourceManager<HVtxShader>::Load_FromKey(L"VSBRECTCOLOR", L"Shader", L"BRectShader.fx", "VS_BDebugRect");
	BRectV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	BRectV->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	BRectV->CreateLayout();
	ResourceManager<HPixShader>::Load_FromKey(L"PSBRECTCOLOR", L"Shader", L"BRectShader.fx", "PS_BDebugRect");

	KPtr<HMaterial> BRECTMAT = ResourceManager<HMaterial>::Create(L"BRECTMAT");
	BRECTMAT->SetVtxShader(L"VSBRECTCOLOR");
	BRECTMAT->SetPixShader(L"PSBRECTCOLOR");


	// LineRenderData
	ResourceManager<HMesh>::Create(L"LINERECTMESH"
		, 4, (UINT)COLORVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrColorVTX
		, 5, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrLineRectIdX
		, IDX16::FM(), D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	KPtr<HVtxShader> RectV = ResourceManager<HVtxShader>::Load_FromKey(L"VSDEBUGRECTCOLOR", L"Shader", L"DebugRectShader.fx", "VS_DebugRect");
	RectV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RectV->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RectV->CreateLayout();
	ResourceManager<HPixShader>::Load_FromKey(L"PSDEBUGRECTCOLOR", L"Shader", L"DebugRectShader.fx", "PS_DebugRect");

	KPtr<HMaterial> DEBUGMAT = ResourceManager<HMaterial>::Create(L"DEBUGRECTMAT");
	DEBUGMAT->SetVtxShader(L"VSDEBUGRECTCOLOR");
	DEBUGMAT->SetPixShader(L"PSDEBUGRECTCOLOR");

	// ColorRenderData
	ResourceManager<HMesh>::Create(L"COLORMESH"
		, 4, (UINT)COLORVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrColorVTX
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrColorIDX
		, IDX16::FM());

	KPtr<HVtxShader> ColV = ResourceManager<HVtxShader>::Load_FromKey(L"VSCOLOR", L"Shader", L"ColorShader.fx", "VS_Color");
	ColV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColV->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColV->CreateLayout();
	ResourceManager<HPixShader>::Load_FromKey(L"PSCOLOR", L"Shader", L"ColorShader.fx", "PS_Color");

	KPtr<HMaterial> COLORMAT = ResourceManager<HMaterial>::Create(L"COLORMAT");
	COLORMAT->SetVtxShader(L"VSCOLOR");
	COLORMAT->SetPixShader(L"PSCOLOR");

	// TexRenderData
	ResourceManager<HMesh>::Create(L"TEXMESH"
		, 4, (UINT)TEXVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrTexVTX
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrTexIDX
		, IDX16::FM());

	KPtr<HVtxShader> TexV = ResourceManager<HVtxShader>::Load_FromKey(L"VSTEX", L"Shader", L"TexShader.fx", "VS_Tex");
	TexV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TexV->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	TexV->CreateLayout();
	ResourceManager<HPixShader>::Load_FromKey(L"PSTEX", L"Shader", L"TexShader.fx", "PS_Tex");

	KPtr<HMaterial> TEXMAT = ResourceManager<HMaterial>::Create(L"TEXMAT");
	TEXMAT->SetVtxShader(L"VSTEX");
	TEXMAT->SetPixShader(L"PSTEX");
	TEXMAT->SetBlend(L"AlphaBlend");

	// ImageData
	KPtr<HVtxShader> ImgV = ResourceManager<HVtxShader>::Load_FromKey(L"VSIMG", L"Shader", L"ImageShader.fx", "VS_Img");
	ImgV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ImgV->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	ImgV->CreateLayout();
	ResourceManager<HPixShader>::Load_FromKey(L"PSIMG", L"Shader", L"ImageShader.fx", "PS_Img");

	KPtr<HMaterial> IMGMAT = ResourceManager<HMaterial>::Create(L"IMGMAT");
	IMGMAT->SetVtxShader(L"VSIMG");
	IMGMAT->SetPixShader(L"PSIMG");
	IMGMAT->SetBlend(L"AlphaBlend");

	//ResourceManager<HBlend>::Create(L"AlphaBlend");
	//ResourceManager<HFont>::Create(L"±Ã¼­", L"±Ã¼­");

	//Core_Class::MainDevice().CreateCB<KMatrix>(L"TRANS", D3D11_USAGE_DYNAMIC, 0);
	//Core_Class::MainDevice().CreateCB<HCOLOR>(L"MULCOLOR", D3D11_USAGE_DYNAMIC, 0);
	//Core_Class::MainDevice().CreateCB<KVector4>(L"IMGUV", D3D11_USAGE_DYNAMIC, 1);

	return true;
}
