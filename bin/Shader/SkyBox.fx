// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"

// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.

Texture2D g_Tex_0 : register(t0);
SamplerState g_Sam_0 : register(s0);

cbuffer MulColor : register(b0)
{
	float4 CBColor;
}


struct VTX3DMESH_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTX3DMESH_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

struct PS3D_OUTPUT
{
	float4 vColor : SV_Target;
};

VTX3DMESH_OUTPUT VS_SKY3D(VTX3DMESH_INPUT _iN)
{
	VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;
	outData.vColor = _iN.vColor;
	outData.vNormal = _iN.vNormal;

	return outData;
}

PS3D_OUTPUT PS_SKY3D(VTX3DMESH_OUTPUT _in)
{
	PS3D_OUTPUT outData = (PS3D_OUTPUT)0.0f;
	outData.vColor = g_Tex_0.Sample(g_Sam_0, float2(_in.vUv.x, _in.vUv.y));
	return outData;
}