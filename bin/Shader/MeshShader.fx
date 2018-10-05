#include "Matrix.fx"
#include "LightContainer.fx"
#include "Texture.fx"

struct MESH_VT_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN;
    float4 vBTangent : BTAN;
};

struct MESH_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN;
    float4 vBTangent : BTAN;
};

struct MESH_PX_OUT
{
    float4 vColor : SV_Target;
};


MESH_VT_OUT Mesh_VT(MESH_VT_IN _in)
{
    MESH_VT_OUT outData = (MESH_VT_OUT) 0.f;

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

MESH_PX_OUT Mesh_PX(MESH_VT_OUT _in)
{
    MESH_PX_OUT outData = (MESH_PX_OUT) 0.0f;

    outData.vColor = _in.vColor;


    float4 CalCor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i< TexCnt; ++i)
    {
        if(-1 != Texes[i].TInx)
        {
            CalCor += Get_CalColor(Texes[i].TInx, Texes[i].TSmp, _in.vUv) * _in.vColor;
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

    outData.vColor.rgb = CalCor.rgb * LColor.rgb;
    outData.vColor.a = _in.vColor.a;

    return outData;
}