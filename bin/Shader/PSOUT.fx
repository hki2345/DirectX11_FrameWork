
struct PS_DEFFERDOUTPUT
{
    float4 vDiffuse     : SV_Target; // »ö±ò.
    float4 vPosition    : SV_Target1; // À§Ä¡
    float4 vNoraml      : SV_Target2; // ³ë¸»
    float4 vDepth       : SV_Target3; // ±íÀÌ ±×¸²ÀÚ.
    // float4 vDepth       : SV_Target4; // ±íÀÌ ±×¸²ÀÚ.
};

struct PS_FORWARDOUTPUT
{
    float4 vDiffuse : SV_Target; // »ö±ò.
};

struct PS_MERGEOUTPUT
{
    float4 vMergeColor : SV_Target; // »ö±ò.
};

struct VS_DEFFERDLIGHTINPUT
{
    float4 vPos: POSITION; // »ö±ò.
    float2 vUv: TEXCOORD; // À§Ä¡
};

struct VS_DEFFERDLIGHTOUTPUT
{
    float4 vPos : SV_POSITION; // »ö±ò.
    float2 vUv : TEXCOORD; // À§Ä¡
};


struct PS_DEFFERDLIGHTOUTPUT
{
    float4 vDiffuse : SV_Target; // È®»ê±¤.
    float4 vSpaculer : SV_Target1; // ¹Ý»ç±¤.
};