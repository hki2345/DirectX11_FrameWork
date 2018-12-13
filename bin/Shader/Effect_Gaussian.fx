#include "GValue.fx"
#include "PSOUT.fx"
#include "GTex.fx"
#include "DefferdLight.fx"


static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

//static float g_vGaussian[9] =
//{
//    1.0f, 2.0f, 1.0f, = 4
//    2.0f, 4.0f, 2.0f, = 8
//    1.0f, 2.0f, 1.0f = 4
//};

// 가우시안 비율 
static float g_vGaussian[25] =
{
    1.0f, 2.0f, 4.0f, 2.0f, 1.0f, // 10
    2.0f, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    4.0f, 8.0f, 16.0f, 8.0f, 4.0f, // 40
    2.0f, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    1.0f, 2.0f, 4.0f, 2.0f, 1.0f // 10
};
// 현재 저거 다 합친 수로 나누면 된다.

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

cbuffer POSTDATA : register(b0)
{
    float4 g_ScreenSize;
}


VTXCOL_OUTPUT VS_GN(VTXCOL_INPUT _In)
{
    VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT) 0.f;
    outData.vPos = mul(_In.vPos, DirMat);
    outData.vUv = _In.vUv;
    return outData;
}

PS_OUTPUT PS_GN(VTXCOL_OUTPUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
    // 축소 버퍼를 만든다. 

    // 화면크기 나누기 1하면 
    float PixelU = 1 / g_ScreenSize.x;
    float PixelV = 1 / g_ScreenSize.y;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            float2 vUV = _in.vUv + float2((i - 1) * PixelU, (j - 1) * PixelV);
            outData.vCol += g_Tex_0.Sample(g_Sam_0, vUV) * g_vGaussian[i * 5 + j];
        }
    }
    // 여기서 매트릭스 나눈거 더하면 됌
    outData.vCol /= 100.0f;

    return outData;
}