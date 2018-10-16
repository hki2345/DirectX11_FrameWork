#include "MatrixContainer.fx"
#include "LightDefferd.fx"
#include "TextureContainer.fx"

struct VOLUME_VT_OUT
{
    float4 vPos : SV_POSITION;
};

VOLUME_VT_OUT Volume_VT(float4 _Pos : POSITION)
{
    VOLUME_VT_OUT TempData = (VOLUME_VT_OUT) .0f;

    TempData.vPos = mul(_Pos, g_WVP);
    return TempData;
}

struct VOLUME_PX_OUT
{
    float4 vColor : SV_Target;
};


VOLUME_PX_OUT Volume_PX(VOLUME_VT_OUT _in)
{
    VOLUME_PX_OUT NoneThis = (VOLUME_PX_OUT) .0f;
    return NoneThis;
}