// 디퍼드느 이렇게 한꺼번에 받는 게 특징임ㅇㅇ
struct PS_DEFFERDOUTPUT
{
    float4 vDiffuse : SV_Target;
    float4 vPosition : SV_Target1;
    float4 vNoraml : SV_Target2;
    float4 vDepth : SV_Target3;
    // float4 vDepth       : SV_Target4; // 깊이 그림자.
};

struct PS_FORWARDOUTPUT
{
    float4 vDiffuse : SV_Target; // 색깔.
};