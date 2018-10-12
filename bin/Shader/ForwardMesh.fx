#include "MatrixContainer.fx"
#include "LightContainer.fx"
#include "PixelContainer.fx"
#include "TextureContainer.fx"

struct FORMESH_VT_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN;
    float4 vBTangent : BTAN;
};

struct FORMESH_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN;
    float4 vBTangent : BTAN;
};


FORMESH_VT_OUT ForMesh_VT(FORMESH_VT_IN _in)
{
    FORMESH_VT_OUT outData = (FORMESH_VT_OUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    outData.vViewPos = mul(_in.vPos, g_WV);
    _in.vNormal.w = 0.0f;
    outData.vNormal = normalize(mul(_in.vNormal, g_WV));
    outData.vTangent = normalize(mul(_in.vTangent, g_WV));
    outData.vBTangent = normalize(mul(_in.vBTangent, g_WV));
    



    if (0 == CheckLight)
    {
        return outData;
    }
    
    // 픽셀인지 구분
    if (1 == VzPo)
    {
        return outData;
    }
    
    if (0 == LightCount)
    {
        return outData;
    }
    
    float3 LColor = (float3) .0f;

    for (int i = 0; i < LightCount; ++i)
    {
        
        if (LightList[i].Type == 0)
        {
            LColor += Direct_Light(outData.vViewPos, _in.vNormal, LightList[i]);
        }
        else if (LightList[i].Type == 1)
        {
            LColor += Point_Light(outData.vViewPos, _in.vNormal, LightList[i]);
        }
    }
    
    LColor /= (float) LightCount;
    
    outData.vColor.rgb = LColor.rgb;
    outData.vColor.a = _in.vColor.a;

    return outData;
}

FORMESH_PX_OUT ForMesh_PX(FORMESH_VT_OUT _in)
{
    FORMESH_PX_OUT outData = (FORMESH_PX_OUT) 0.0f;

    outData.vDiffuse = _in.vColor;
    float4 CalCor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < TexCnt; ++i)
    {
        if(-1 != Texes[i].TInx)
        {
            if (0 == Texes[i].Type)
            {
                 CalCor *= Get_CalColor(Texes[i].TInx, Texes[i].TSmp, _in.vUv) * _in.vColor;
            }
            else if (1 == Texes[i].Type)
            {
                _in.vNormal = Set_Bump(Texes[i].TInx, Texes[i].TSmp, _in.vUv, _in.vTangent, _in.vBTangent, _in.vNormal);
            }

            // 쉐이더는 else를 하면 안 된다이으.ㅠ
        }
    }

    // 계산.
    if (0 == CheckLight)
    {
        return outData;
    }

    // 버텍스인지 구분
    if (0 == VzPo)
    {
        return outData;
    }

    if (0 == LightCount)
    {
        return outData;
    }
    


    float3 LColor = (float3) .0f;

    for (int i = 0; i < LightCount; ++i)
    {
        if (LightList[i].Type == 0)
        {
            LColor += Direct_Light(_in.vViewPos, _in.vNormal, LightList[i]);
        }
        else if (LightList[i].Type == 1)
        {
            LColor += Point_Light(_in.vViewPos, _in.vNormal, LightList[i]);
        }
    }

    LColor /= (float) LightCount;

    outData.vDiffuse.rgb = CalCor.rgb * LColor.rgb;
    outData.vDiffuse.a = _in.vColor.a;

    return outData;
}