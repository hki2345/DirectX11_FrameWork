// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "MatrixContainer.fx"
#include "PixelContainer.fx"
#include "LightDefferd.fx"
#include "TextureContainer.fx"

struct DEFMESH_VT_IN
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
struct DEFMESH_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN; // U와 일치하는 X축
    float4 vBNormal : BTAN; // V와 일치하는 탄젠트
};


DEFMESH_VT_OUT DefMesh_VT(DEFMESH_VT_IN _in)
{
    DEFMESH_VT_OUT outData = (DEFMESH_VT_OUT) 0.f;

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

DEFMESH_PX_OUT DefMesh_PX(DEFMESH_VT_OUT _in)
{
    DEFMESH_PX_OUT outData = (DEFMESH_PX_OUT) 0.0f;
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
    outData.vDiffuse.rgb = CalColor.rgb;
    outData.vDiffuse.a = _in.vColor.a;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.xyzw = outData.vPosition.z;

    return outData;
}


// .5f 로 들어오니까 2배로 만들어야 한다.
static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

DEFFERDLIGHT_VT_OUT DefLight_VT(DEFFERDLIGHT_VT_IN _Input)
{
    DEFFERDLIGHT_VT_OUT OUTDATA = (DEFFERDLIGHT_VT_OUT) 0.0F;
    OUTDATA.vPos = mul(_Input.vPos, DirMat);
    OUTDATA.vUv = _Input.vUv;
    return OUTDATA;
}


DEFFERDLIGHT_PX_OUT DefLight_PX(DEFFERDLIGHT_VT_OUT _Input)
{
    DEFFERDLIGHT_PX_OUT OUTDATA = (DEFFERDLIGHT_PX_OUT) 0.0F;

    float fDepth = g_Tex_2.Sample(g_Sam_0, _Input.vUv).x;
    if (fDepth == 0.0f)
    {
        clip(-1);
    }

    float4 vViewPos = g_Tex_0.Sample(g_Sam_0, _Input.vUv);
    float4 vNormal = g_Tex_1.Sample(g_Sam_0, _Input.vUv);

    LightColor info = Direct_Light(vViewPos, vNormal, LD);

    OUTDATA.vDiffuse.rgb = info.Diff.rgb;
    OUTDATA.vDiffuse.a = 1.0f;
    OUTDATA.vSpeculer.rgb = info.Spec.rgb + float3(.1f, .1f, .1f);
    OUTDATA.vSpeculer.a = 1.0f;

    return OUTDATA;
}

// 최종 병합 쉐이더 - 리소스 -> 리소스와 렌더는 같이 되지 않는다.
DEFFERDLIGHT_VT_OUT DefMerge_VT(DEFFERDLIGHT_VT_IN _Input)
{
    DEFFERDLIGHT_VT_OUT OUTDATA = (DEFFERDLIGHT_VT_OUT) 0.0F;
    OUTDATA.vPos = mul(_Input.vPos, DirMat);
    OUTDATA.vUv = _Input.vUv;
    return OUTDATA;
}

MERGE_PX_OUT DefMerge_PX(DEFFERDLIGHT_VT_OUT _Input)
{
    MERGE_PX_OUT OUTDATA = (MERGE_PX_OUT) 0.0F;
    float4 vColor = g_Tex_0.Sample(g_Sam_0, _Input.vUv);
    float4 vDiff = g_Tex_1.Sample(g_Sam_0, _Input.vUv);
    float4 vSpec = g_Tex_2.Sample(g_Sam_0, _Input.vUv);
    
    vColor.w = 1.0f;
    vDiff.w = 1.0f;
    vSpec.w = 0.0f;
    
    
    if (OUTDATA.vMergeColor.a != 0.f)
    {
        OUTDATA.vMergeColor.rgb = float3(0.2f, 0.2f, 0.8f) * vColor.a;
    }

    OUTDATA.vMergeColor.rgb += vColor.rgb * vDiff.rgb + vSpec.rgb /*+ float3(0.1f, 0.1f, 0.1f)*/;
    OUTDATA.vMergeColor.a = 1.0f;

    return OUTDATA;
}