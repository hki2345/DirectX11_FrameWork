#include "GValue.fx"
#include "PSOUT.fx"
#include "DefferdLight.fx"
#include "GTex.fx"



/************************** ���ؽ� ******************************/
struct EFFECT_INPUT
{
    float4 vPos : POSITION;
    uint vSwitch : SWITCH;
};


// ���ؽ����� �̰��� ������� ���� �Ǻ����� - ���ؽ��� �ϴ� �� ����
EFFECT_INPUT VS_EFFECTFILTER(EFFECT_INPUT _Data)
{
    return _Data;
};


/************************** ������Ʈ�� ******************************/
[maxvertexcount(2)]
void GS_EFFECTFILTER(point EFFECT_INPUT _Data[1], inout PointStream<EFFECT_INPUT> _EStream)
{
   // ���ڿ��ٰ� �ٿ��� �ִ´�.
    // �� �̹��� ��� ���ڰ� �������� �����Ѵ�.
    //if (_Data[0].vOnOff == 0)
    //{
    EFFECT_INPUT NEF = (EFFECT_INPUT) 0.0f;
    NEF.vPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _EStream.Append(NEF);
    //}

    // ��Ʈ���� �ѱ�� �װ� �ٽ� ���̴� ������ ���͸��� ���ؽ�����
    // �׸������� �ٽ� �ִ´�.
}

/************************** �� ���ε� - ���� �׸��� ���� ******************************/
struct EFFECT_VSOUT
{
    float4 vPos : POSITION;
};

struct EFFECT_GSOUT
{
    float4 vPos : SV_POSITION;
};


EFFECT_VSOUT VS_EFFECTRENDER(EFFECT_INPUT _Data)
{
    EFFECT_VSOUT Tmp = (EFFECT_VSOUT) .0f;
    Tmp.vPos = _Data.vPos;
    return Tmp;
}


/************************** ������Ʈ�� ******************************/
// ���⼭ 4��� ���ڴ� �簢�����׸��ڴٴ� ��
[maxvertexcount(4)]
void GS_EFFECTRENDER(point EFFECT_INPUT _Data[1], inout TriangleStream<EFFECT_GSOUT> _TStream)
{
    float3 Up = float3(.0f, 1.0f, .0f);
    float3 Right = float3(1.0f, .0f, .0f);

    float4 vTri[4];
    vTri[0] = float4(_Data[0].vPos.xyz + Right - Up, 1.0f);
    vTri[1] = float4(_Data[0].vPos.xyz + Right + Up, 1.0f);
    vTri[2] = float4(_Data[0].vPos.xyz - Right - Up, 1.0f);
    vTri[3] = float4(_Data[0].vPos.xyz - Right + Up, 1.0f);
                                                     

    EFFECT_GSOUT NVtx = (EFFECT_GSOUT) .0f;


    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        NVtx.vPos = mul(vTri[i], g_WVP);
        _TStream.Append(NVtx);
    }

    return;
}


/************************** �ȼ� ******************************/
// ������Ʈ������ ���� �� �ȼ��� �׸���.
float4 PS_EFFECTRENDER(EFFECT_GSOUT _Data) : SV_Target
{
    return float4(1.0f, .0f, .0f, 1.0f);
}
