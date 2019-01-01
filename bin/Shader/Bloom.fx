#include "GValue.fx"
#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

static float g_vGaussian[25] =
{
    0.0f, 0.0f, 8.0f, 0.0f, 0.0f, // 8
    0.0f, 0.0f, 16.0f, 0.0f, 0.0f, // 16
    8.0f, 16.0f, 16.0f, 16.0f, 8.0f, // 64
    0.0f, 0.0f, 16.0f, 0.0f, 0.0f, // 16
    0.0f, 0.0f, 8.0f, 0.0f, 0.0f // 8 // 112
};

struct VTXCOL_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

struct VTXCOL_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 vCol : SV_Target;
};


cbuffer SCREENSIZE : register(b0)
{
    float4 g_ScreenSize;
}

cbuffer TARGETSIZE : register(b1)
{
    float4 g_TargetSize;
}


static matrix CalMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

VTXCOL_OUTPUT VS_EX(VTXCOL_INPUT _In)
{
    VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT) 0.f;
    outData.vPos = mul(_In.vPos, CalMat);

    _In.vUv.x *= (g_ScreenSize.x / g_TargetSize.x);
    _In.vUv.y *= (g_ScreenSize.y / g_TargetSize.y);

    outData.vUv = _In.vUv;
    return outData;
}

PS_OUTPUT PS_EX(VTXCOL_OUTPUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;

    outData.vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);

    if (0.66f > ((outData.vCol.x + outData.vCol.y + outData.vCol.z) / 3.0f))
    {
        outData.vCol = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    return outData;
}

VTXCOL_OUTPUT VS_SMALL(VTXCOL_INPUT _In)
{
    VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT) 0.f;
    outData.vPos = mul(_In.vPos, CalMat);

    _In.vUv.x *= (g_ScreenSize.x / g_TargetSize.x);
    _In.vUv.y *= (g_ScreenSize.y / g_TargetSize.y);

    outData.vUv = _In.vUv;
    return outData;
}

PS_OUTPUT PS_SMALL(VTXCOL_OUTPUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;

    outData.vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);
    return outData;
}

//     g_tex0 // �ƿ�ǲ Ÿ��
// �ϴ� ������ �ʰ�.
//     g_tex1 // ����Ÿ��

VTXCOL_OUTPUT VS_BLOOMMERGE(VTXCOL_INPUT _In)
{
    VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT) 0.f;
    outData.vPos = mul(_In.vPos, DirMat);
    outData.vUv = _In.vUv;
    return outData;
}

PS_OUTPUT PS_BLOOMMERGE(VTXCOL_OUTPUT _in)
{
    // ��� ���۸� �����. 
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;

    float Color = g_Tex_1.Sample(g_Sam_0, _in.vUv);

    float PixelU = 1 / g_ScreenSize.x;
    float PixelV = 1 / g_ScreenSize.y;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            float2 vUV = _in.vUv + float2((i - 1) * PixelU, (j - 1) * PixelV);
            Color += g_Tex_1.Sample(g_Sam_0, vUV) * g_vGaussian[i * 5 + j];
        }
    }

    Color /= 2500.0f;


    // g_Tex_0�� ����Ʈ ��ǻ�� Ÿ���� ���Դ�.
    outData.vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);
    outData.vCol += Color;
    outData.vCol.b += Color;


    return outData;
}