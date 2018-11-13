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


/************************** ���ؽ� ******************************/
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

/************************** �� ******************************/
struct TesselFactor
{
    float EdgeArr[3] : SV_TessFactor;
    float fInsideFactor : SV_InsideTessFactor;
};

// �׼����̼��� ���� ���� ���ϴ� �Լ�
// ����������� �Ի��� �Ѵ�.
// �븻�̳� ���̳븻�� �̰� ����� �ٽ�  ����� �Ǿ�� ��
// ¥�� ���ؽ��� �ϳ� ���� ����� �����̱� ������ ���⼭ �޶���
// �׸��� ������ �� �� ������ �������� �� ������ �� �Ÿ��� �ּҰ�, �ִ밪, �� �׼��� �ִ� �ּ� ���� ����
float Cal_TessLv(float _fMinDist, float _fMaxDist, float _fMinTess, float _fMaxTess, float3 _vWorldPos)
{
    float3 vDis = _vWorldPos;
    vDis.y = .0f;
    float fDist = length(vDis.xyz);
    float fRatio = (fDist - _fMinDist) / (_fMaxDist - _fMinDist);


    return pow(2, saturate(floor(lerp(4, 0, fRatio)) / 4.0f) * 4.0f);

}

// ���ؽ� ������ 3���� �޴´� - �̰ɷ� �ﰢ���� �����ϱ� ������
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


/************************** ��¥ �� ���̴� ******************************/
// �濡�� �Ļ��Ǵ�(���Ǵ�) �� 2���� �ִµ� �� ���ϳ��� ������ �ٽ� �ѹ� ����ϰڴٴ�
// ���̴�. �� ������ ���� �����ְ�, 




/************************** �ȼ� ******************************/
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