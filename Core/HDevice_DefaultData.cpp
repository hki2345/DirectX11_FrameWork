#include "HDevice.h"
#include "DVHeader.h"
#include "HWindow.h"
#include "Stl_AID.h"
#include "HMesh.h"
#include "Core_Class.h"
#include "HVtxShader.h"
#include "HPixShader.h"
#include "HResMgr.h"
#include "HMaterial.h"
#include "HBlend.h"
#include "HFont.h"

bool HDevice::DefaultDataInit() 
{
	HResMgr<HBlend>::Create(L"AlphaBlend");
	HResMgr<HFont>::Create(L"±Ã¼­", L"±Ã¼­");

	// Color Vtx
	COLORVTX ArrColorVTX[4] = {};
	IDX16 ArrColorIDX[2] = {};
	ArrColorVTX[0].Pos = HVEC(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrColorVTX[1].Pos = HVEC(0.5f, 0.5f, 0.0f, 1.0f);
	ArrColorVTX[2].Pos = HVEC(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrColorVTX[3].Pos = HVEC(0.5f, -0.5f, 0.0f, 1.0f);

	ArrColorVTX[0].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[1].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[2].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);
	ArrColorVTX[3].Color = HVEC(1.0f, 1.0f, 1.0f, 1.0f);

	ArrColorIDX[0] = IDX16(0, 3, 2);
	ArrColorIDX[1] = IDX16(0, 1, 3);

	WORD ArrLineRectIdX[5] = { 0, 1, 3, 2, 0 };



	// Tex Vtx
	TEXVTX ArrTexVTX[4] = {};
	IDX16 ArrTexIDX[2] = {};
	ArrTexVTX[0].Pos = HVEC(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVTX[1].Pos = HVEC(0.5f, 0.5f, 0.0f, 1.0f);
	ArrTexVTX[2].Pos = HVEC(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrTexVTX[3].Pos = HVEC(0.5f, -0.5f, 0.0f, 1.0f);

	ArrTexVTX[0].Uv = HVEC2(0.0f, 0.0f);
	ArrTexVTX[1].Uv = HVEC2(1.0f, 0.0f);
	ArrTexVTX[2].Uv = HVEC2(0.0f, 1.0f);
	ArrTexVTX[3].Uv = HVEC2(1.0f, 1.0f);

	ArrTexIDX[0] = IDX16(0, 3, 2);
	ArrTexIDX[1] = IDX16(0, 1, 3);


	// Boder Rect Shader
	KPtr<HVtxShader> BRectV = HResMgr<HVtxShader>::LoadToKey(L"VSBRECTCOLOR", L"Shader", L"BRectShader.fx", "VS_BDebugRect");
	BRectV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	BRectV->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	BRectV->CreateLayout();
	HResMgr<HPixShader>::LoadToKey(L"PSBRECTCOLOR", L"Shader", L"BRectShader.fx", "PS_BDebugRect");

	KPtr<HMaterial> BRECTMAT = HResMgr<HMaterial>::Create(L"BRECTMAT");
	BRECTMAT->SetVtxShader(L"VSBRECTCOLOR");
	BRECTMAT->SetPixShader(L"PSBRECTCOLOR");


	// LineRenderData
	HResMgr<HMesh>::Create(L"LINERECTMESH"
		, 4, (UINT)COLORVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrColorVTX
		, 5, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrLineRectIdX
		, IDX16::FM(), D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	KPtr<HVtxShader> RectV = HResMgr<HVtxShader>::LoadToKey(L"VSDEBUGRECTCOLOR", L"Shader", L"DebugRectShader.fx", "VS_DebugRect");
	RectV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RectV->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RectV->CreateLayout();
	HResMgr<HPixShader>::LoadToKey(L"PSDEBUGRECTCOLOR", L"Shader", L"DebugRectShader.fx", "PS_DebugRect");

	KPtr<HMaterial> DEBUGMAT = HResMgr<HMaterial>::Create(L"DEBUGRECTMAT");
	DEBUGMAT->SetVtxShader(L"VSDEBUGRECTCOLOR");
	DEBUGMAT->SetPixShader(L"PSDEBUGRECTCOLOR");

	// ColorRenderData
	HResMgr<HMesh>::Create(L"COLORMESH"
		, 4, (UINT)COLORVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrColorVTX
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrColorIDX
		, IDX16::FM());

	KPtr<HVtxShader> ColV = HResMgr<HVtxShader>::LoadToKey(L"VSCOLOR", L"Shader", L"ColorShader.fx", "VS_Color");
	ColV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColV->AddLayout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ColV->CreateLayout();
	HResMgr<HPixShader>::LoadToKey(L"PSCOLOR", L"Shader", L"ColorShader.fx", "PS_Color");

	KPtr<HMaterial> COLORMAT = HResMgr<HMaterial>::Create(L"COLORMAT");
	COLORMAT->SetVtxShader(L"VSCOLOR");
	COLORMAT->SetPixShader(L"PSCOLOR");

	// TexRenderData
	HResMgr<HMesh>::Create(L"TEXMESH"
		, 4, (UINT)TEXVTX::TypeSize(), D3D11_USAGE_DYNAMIC, ArrTexVTX
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrTexIDX
		, IDX16::FM());

	KPtr<HVtxShader> TexV = HResMgr<HVtxShader>::LoadToKey(L"VSTEX", L"Shader", L"TexShader.fx", "VS_Tex");
	TexV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TexV->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	TexV->CreateLayout();
	HResMgr<HPixShader>::LoadToKey(L"PSTEX", L"Shader", L"TexShader.fx", "PS_Tex");

	KPtr<HMaterial> TEXMAT = HResMgr<HMaterial>::Create(L"TEXMAT");
	TEXMAT->SetVtxShader(L"VSTEX");
	TEXMAT->SetPixShader(L"PSTEX");
	TEXMAT->SetBlend(L"AlphaBlend");

	// ImageData
	KPtr<HVtxShader> ImgV = HResMgr<HVtxShader>::LoadToKey(L"VSIMG", L"Shader", L"ImageShader.fx", "VS_Img");
	ImgV->AddLayout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	ImgV->AddLayout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	ImgV->CreateLayout();
	HResMgr<HPixShader>::LoadToKey(L"PSIMG", L"Shader", L"ImageShader.fx", "PS_Img");

	KPtr<HMaterial> IMGMAT = HResMgr<HMaterial>::Create(L"IMGMAT");
	IMGMAT->SetVtxShader(L"VSIMG");
	IMGMAT->SetPixShader(L"PSIMG");
	IMGMAT->SetBlend(L"AlphaBlend");

	//HResMgr<HBlend>::Create(L"AlphaBlend");
	//HResMgr<HFont>::Create(L"±Ã¼­", L"±Ã¼­");

	//Core_Class::MainDevice().CreateCB<HMAT>(L"TRANS", D3D11_USAGE_DYNAMIC, 0);
	//Core_Class::MainDevice().CreateCB<HCOLOR>(L"MULCOLOR", D3D11_USAGE_DYNAMIC, 0);
	//Core_Class::MainDevice().CreateCB<HVEC>(L"IMGUV", D3D11_USAGE_DYNAMIC, 1);

	return true;
}
