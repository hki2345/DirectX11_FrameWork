// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "GTex.fx"

cbuffer TerrainBuffer : register(b0)
{
    int FloorCount;
    int VTXX;
    int VTXY;
    int temp3;
    int IsBump[4];
}

struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BINORMAL; // V�� ��ġ�ϴ� ź��Ʈ
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct VTX3DMESH_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BINORMAL; // V�� ��ġ�ϴ� ź��Ʈ
};


VTX3DMESH_OUTPUT VS_TERRAINDEFFERD(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    outData.vViewPos = mul(_in.vPos, g_WV);
    outData.vNormal = normalize(mul(_in.vNormal, g_WV));
    outData.vTangent = normalize(mul(_in.vTangent, g_WV));
    outData.vBNormal = normalize(mul(_in.vBNormal, g_WV));

    if (0 == IsLight)
    {
        return outData;
    }

    return outData;
}

PS_DEFFERDOUTPUT PS_TERRAINDEFFERD(VTX3DMESH_OUTPUT _in)
{
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    outData.vDiffuse = _in.vColor;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    CalColor *= GetMTexToColor(8, 8, _in.vUv, 0.0f);
    _in.vNormal = CalMBump(8, 8, _in.vUv, 1.0f, _in.vTangent, _in.vBNormal, _in.vNormal);
    
    float2 SpUv;

    SpUv.x = _in.vUv.x / VTXX;
    SpUv.y = _in.vUv.y / VTXY;

    for (int i = 0; i < FloorCount; ++i)
    {
        // ���� ����.
        float4 Ratio = GetTexToColor(i, i, SpUv);
        float RatioValuie = (Ratio.x + Ratio.y + Ratio.z) / 3.0f;
        float4 FloorColor = GetMTexToColor(9 + i, 9 + i, _in.vUv, 0.0f);
        float4 SrcColor = CalColor;
        FloorColor.xyz *= RatioValuie;
        SrcColor.xyz *= (1.0f - Ratio.x);
        CalColor = FloorColor + SrcColor;
        if (RatioValuie >= 0.9)
        {
            _in.vNormal = CalMBump(9 + i, 9 + i, _in.vUv, 1.0f, _in.vTangent, _in.vBNormal, _in.vNormal);
        }
    }

    //CalColor *= GetTexToColor(0, 0, _in.vUv);
    //_in.vNormal = CalBump(1, 0, _in.vUv, _in.vTangent, _in.vBNormal, _in.vNormal);
    // ������� �ϳ��� ������ �Ѵ�.

    //// ������ 2

    //for (int i = 0; i < FloorCount; ++i)
    //{

    //}

    //for (int i = 0; i < FloorCount; ++i)
    //{
    //    float SPRatio = 0.1;

    //    float3 DestColor = CalColor * (1 - SPRatio);
    //    float3 TexColor;
    //    float3 SrcColor;
    //    SrcColor *= TexColor * (SPRatio);
    //    CalColor = DestColor + SrcColor;
    //}

    // BaseDiffTextureColor 

    // 0.5 x
    // 1, 1, 1,
    // rgb

    // Į �÷��� ���ΰ����� ���;� �Ѵ�.

    // ������ ������ �ƿ� ������� �ʴ� ���� �ƴϴ�.
    outData.vDiffuse.rgb = CalColor;
    outData.vDiffuse.a = _in.vColor.a;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.x = outData.vPosition.z;
    outData.vDepth.w = 1.0f;

    return outData;
}