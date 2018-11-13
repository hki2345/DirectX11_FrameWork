// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
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


/************************** 버텍스 ******************************/
struct VTX3DMESH_INPUT
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT; // U와 일치하는 X축
    float4 vBNormal : BINORMAL; // V와 일치하는 탄젠트
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
    float4 vTangent : TANGENT; // U와 일치하는 X축
    float4 vBNormal : BINORMAL; // V와 일치하는 탄젠트
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

/************************** 헐 ******************************/
struct TesselFactor
{
    float EdgeArr[3] : SV_TessFactor;
    float fInsideFactor : SV_InsideTessFactor;
};

// 테셀레이션의 보간 값을 구하는 함수
// 월드기준으로 게산을 한다.
// 노말이나 바이노말은 이거 계산후 다시  계산이 되어야 함
// 짜피 버텍스가 하나 새로 생기는 구현이기 때문에 여기서 달라짐
// 그리고 가까이 갈 때 무조건 나눠버릴 수 있으니 그 거리의 최소값, 최대값, 및 테셀의 최대 최소 까지 존재
float Cal_TessLv(float _fMinDist, float _fMaxDist, float _fMinTess, float _fMaxTess, float3 _vWorldPos)
{
    float3 vDis = _vWorldPos;
    vDis.y = .0f;
    float fDist = length(vDis.xyz);
    float fRatio = (fDist - _fMinDist) / (_fMaxDist - _fMinDist);


    return pow(2, saturate(floor(lerp(4, 0, fRatio)) / 4.0f) * 4.0f);

}

// 버텍스 데이터 3개를 받는다 - 이걸로 삼각형을 구성하기 때문에
TesselFactor HS_CONSTANTDATA(InputPatch<VTX3DMESH_OUTPUT,  3> _VtxData)
{
    TesselFactor Temp = (TesselFactor) .0f;

    float3 vStdPos = (_VtxData[0].vPos.xyz + _VtxData[1].vPos.xyz) / 2.0f;
    float XRatio = _VtxData[2].vPos.x - _VtxData[1].vPos.x;
    float ZRatio = _VtxData[2].vPos.z - _VtxData[0].vPos.z;


    float3 XPos = float3(vStdPos.x + XRatio, vStdPos.y, vStdPos.z);
    float3 ZPos = float3(vStdPos.x, vStdPos.y, vStdPos.z + ZRatio);

    float FTess = Cal_TessLv(500.0f, 5000.0f, .0f, .0f, vStdPos);
    float FXTess = Cal_TessLv(500.0f, 5000.0f, .0f, .0f, XPos);
    float FZTess = Cal_TessLv(500.0f, 5000.0f, .0f, .0f, ZPos);



    if(FTess < FZTess)
    {
        Temp.EdgeArr[0] = FZTess;
    }
    else
    {
        Temp.EdgeArr[0] = FTess;
    }

    if (FTess < FXTess)
    {
        Temp.EdgeArr[1] = FXTess;
    }
    else
    {
        Temp.EdgeArr[1] = FTess;
    }

    Temp.EdgeArr[2] = FTess;
    Temp.fInsideFactor = FTess;

    return Temp;
}

struct HULL_OUT
{
    float3 vPos : POSITION;
    float3 vUV : TEXCOORD;
};


/************************** 진짜 헐 세이더 ******************************/
// 헐에서 파생되는(계산되는) 게 2가지 있는데 그 중하나를 위에서 다시 한번 계산하겠다는
// 뜻이다. 그 과정이 위에 적혀있고, 




/************************** 픽셀 ******************************/
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
        // 색깔 섞기.
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
    // 상수버퍼 하나를 만들어야 한다.

    //// 색깔을 2

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

    // 칼 컬러가 섞인것으로 나와야 한다.

    // 포워드 색깔을 아예 사용하지 않는 것은 아니다.
    outData.vDiffuse.rgb = CalColor;
    outData.vDiffuse.a = _in.vColor.a;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.x = outData.vPosition.z;
    outData.vDepth.w = 1.0f;

    return outData;
}