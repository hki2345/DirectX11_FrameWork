#include "Matrix.fx"

struct NONE_VT_OUT
{
    float4 vPos : SV_POSITION;
};

NONE_VT_OUT None_VT(float4 _Pos : POSITION)
{
    NONE_VT_OUT NoneThis = (NONE_VT_OUT) .0f;

    NoneThis.vPos = mul(_Pos, g_WVP);
    return NoneThis;
}

struct NONE_PX_OUT
{
    float4 vColor : SV_Target;
};


NONE_PX_OUT None_PX(float4 _in : SV_POSITION)
{
    NONE_PX_OUT NoneThis = (NONE_PX_OUT) .0f;

    NoneThis.vColor = float4(1.0f, .0f, 1.0f, 1.0f);
    return NoneThis;
}