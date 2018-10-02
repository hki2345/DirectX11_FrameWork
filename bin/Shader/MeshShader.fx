#include "Matrix.fx"
#include "LightContainer.fx"

struct MESH_VT_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
};

struct MESH_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
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
    outData.vViewPos = mul(mul(_in.vPos, g_W), g_V);
    _in.vNormal.w = 0.0f;
    outData.vNormal = normalize(mul(mul(_in.vNormal, g_W), g_V));

    if (0 == CheckLight)
    {
        return outData;
    }

    if (1 == VzPo)
    {
        return outData;
    }

    if (0 == LightCount)
    {
        return outData;
    }

    LightColor CalLColor;
    CalLColor.Ambi = (float4) 0.0f;
    CalLColor.Diff = (float4) 0.0f;
    CalLColor.Spec = (float4) 0.0f;

    for (int i = 0; i < LightCount; ++i)
    {
        LightColor LColor = Direct_Light(outData.vViewPos, outData.vNormal, LightList[0]);
        CalLColor.Diff += LColor.Diff;
        CalLColor.Spec += LColor.Spec;
        CalLColor.Ambi += LColor.Ambi;
    }

    CalLColor.Diff /= (float) LightCount;
    CalLColor.Spec /= (float) LightCount;
    CalLColor.Ambi /= (float) LightCount;

    outData.vColor.rgb = _in.vColor.rgb * CalLColor.Diff.rgb + CalLColor.Spec.rgb + CalLColor.Ambi.rgb;
    outData.vColor.a = _in.vColor.a;

    return outData;
}

MESH_PX_OUT Mesh_PX(MESH_VT_OUT _in)
{
    MESH_PX_OUT outData = (MESH_PX_OUT) 0.0f;

    outData.vColor = _in.vColor;

    // °è»ê.
    if (0 == CheckLight)
    {
        return outData;
    }

    if (0 == VzPo)
    {
        return outData;
    }

    if (0 == LightCount)
    {
        return outData;
    }

    LightColor CalLColor;
    CalLColor.Ambi = (float4) 0.0f;
    CalLColor.Diff = (float4) 0.0f;
    CalLColor.Spec = (float4) 0.0f;

    for (int i = 0; i < LightCount; ++i)
    {
        LightColor LColor = Direct_Light(_in.vViewPos, _in.vNormal, LightList[i]);
        CalLColor.Diff += LColor.Diff;
        CalLColor.Spec += LColor.Spec;
        CalLColor.Ambi += LColor.Ambi;
    }

    //CalLColor.Diff /= (float) LightCount;
    //CalLColor.Spec /= (float) LightCount;
    //CalLColor.Ambi /= (float) LightCount;

    outData.vColor.rgb = _in.vColor.rgb * CalLColor.Diff.rgb + CalLColor.Spec.rgb + CalLColor.Ambi.rgb;
    outData.vColor.a = _in.vColor.a;

	// outData.vColor = _in.vColor;
    return outData;
}