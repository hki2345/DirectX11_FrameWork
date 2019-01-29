// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
#include "PSOUT.fx"
#include "ForwardLight.fx"
#include "GTex.fx"

struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTX3DMESH_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};


VTX3DMESH_OUTPUT VS_ANIEFFECT(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;

    return outData;
}


cbuffer ANIEFFECTCB : register(b0)
{
    float4 Color;
    float2 vStart = float2(0.0f, 0.0f);
    float2 vSize = float2(1.0f, 1.0f);
}



PS_FORWARDOUTPUT PS_ANIEFFECT(VTX3DMESH_OUTPUT _in)
{
    PS_FORWARDOUTPUT outData = (PS_FORWARDOUTPUT) 0.0f;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    float4 vCol = g_Tex_0.Sample(g_Sam_0,
		float2(vStart.x + (vSize.x * _in.vUv.x)
			, vStart.y + (vSize.y * _in.vUv.y)));

    // float4 vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);

    if (0 >= vCol.a)
    {
        clip(-1);
    }

    outData.vDiffuse = vCol * Color;


    return outData;
}