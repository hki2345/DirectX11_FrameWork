// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "Matrix.fx"
#include "PixelContainer.fx"
#include "LightContainer.fx"
#include "Texture.fx"

struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN; // U와 일치하는 X축
    float4 vBNormal : BTAN; // V와 일치하는 탄젠트
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTX3DMESH_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN; // U와 일치하는 X축
    float4 vBNormal : BTAN; // V와 일치하는 탄젠트
};


VTX3DMESH_OUTPUT VS_DEFFERD(VTX3DMESH_INPUT _in)
{
    VTX3DMESH_OUTPUT outData = (VTX3DMESH_OUTPUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    outData.vViewPos = mul(_in.vPos, g_WV);
    outData.vNormal = normalize(mul(_in.vNormal, g_WV));
    outData.vTangent = normalize(mul(_in.vTangent, g_WV));
    outData.vBNormal = normalize(mul(_in.vBNormal, g_WV));

    if (0 == CheckLight)
    {
        return outData;
    }

    return outData;
}

PS_DEFFERDOUTPUT PS_DEFFERD(VTX3DMESH_OUTPUT _in)
{
    PS_DEFFERDOUTPUT outData = (PS_DEFFERDOUTPUT) 0.0f;
    outData.vDiffuse = _in.vColor;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    for (int i = 0; i < TexCnt; ++i)
    {
        if (-1 != Texes[i].TInx)
        {
            if (Texes[i].Type == TEXCOLOR)
            {
                CalColor *= Get_CalColor(Texes[i].TInx, Texes[i].TSmp, _in.vUv) * _in.vColor;
            }
            else if (Texes[i].Type == TEXBUMP)
            {
                _in.vNormal = Set_Bump(Texes[i].TInx, Texes[i].TSmp, _in.vUv, _in.vTangent, _in.vBNormal, _in.vNormal);
            }
        }
    }

    // 포워드 색깔을 아예 사용하지 않는 것은 아니다.
    outData.vDiffuse.rgb = CalColor;
    outData.vDiffuse.a = _in.vColor.a;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.xyzw = outData.vPosition.z;

	// outData.vColor = _in.vColor;
    return outData;
}