

// 샘플러와 텍스처를 의미가 다르다.
// 샘플러는 세팅을 안하면 오류를 뱉는다.
// 함

#define TEX 0
#define BUMP 1

Texture2D g_Tex_0 : register(t0);
Texture2D g_Tex_1 : register(t1);
Texture2D g_Tex_2 : register(t2);
Texture2D g_Tex_3 : register(t3);
Texture2D g_Tex_4 : register(t4);
Texture2D g_Tex_5 : register(t5);
Texture2D g_Tex_6 : register(t6);
Texture2D g_Tex_7 : register(t7);

SamplerState g_Sam_0 : register(s0);
SamplerState g_Sam_1 : register(s1);
SamplerState g_Sam_2 : register(s2);
SamplerState g_Sam_3 : register(s3);
SamplerState g_Sam_4 : register(s4);
SamplerState g_Sam_5 : register(s5);
SamplerState g_Sam_6 : register(s6);
SamplerState g_Sam_7 : register(s7);

//Texture2D GetTex(int Slot)
//{
//}

float4 GetColor(Texture2D _Tex, int _Smp, float2 _Uv)
{
    switch (_Smp)
    {
        case 0:
            return _Tex.Sample(g_Sam_0, _Uv);
        case 1:
            return _Tex.Sample(g_Sam_1, _Uv);
        case 2:
            return _Tex.Sample(g_Sam_2, _Uv);
        case 3:
            return _Tex.Sample(g_Sam_3, _Uv);
        case 4:
            return _Tex.Sample(g_Sam_4, _Uv);
        case 5:
            return _Tex.Sample(g_Sam_5, _Uv);
        case 6:
            return _Tex.Sample(g_Sam_6, _Uv);
        case 7:
            return _Tex.Sample(g_Sam_7, _Uv);
        default:
            return (float4)0.0f;
    }
}

float4 GetTexToColor(int _Tex, int _Smp, float2 _Uv)
{
    switch (_Tex)
    {
        case 0:
            return GetColor(g_Tex_0, _Smp, _Uv);
        case 1:
            return GetColor(g_Tex_1, _Smp, _Uv);
        case 2:
            return GetColor(g_Tex_2, _Smp, _Uv);
        case 3:
            return GetColor(g_Tex_3, _Smp, _Uv);
        case 4:
            return GetColor(g_Tex_4, _Smp, _Uv);
        case 5:
            return GetColor(g_Tex_5, _Smp, _Uv);
        case 6:
            return GetColor(g_Tex_6, _Smp, _Uv);
        case 7:
            return GetColor(g_Tex_7, _Smp, _Uv);
        default:
            return (float4)0.0f;
    }
}

//                                          x                    y
float4 CalBump(int _Bumpidx, int _BumpSmp, float2 _Uv, float4 _ViewTangent, float4 _ViewBNormal, float4 _Normal)
{
    float4 BumpNormal = GetTexToColor(_Bumpidx, _BumpSmp, _Uv);

    BumpNormal = BumpNormal * 2.0f - 1.0f; // 0.0 ~ 1.0f -> -1.0f ~ 1.0f
    BumpNormal = normalize(BumpNormal);
    BumpNormal.w = 0.0f;

    // 탄젠트 공간행렬이라고 한다.
    float3x3 matTBN = float3x3(_ViewTangent.xyz, _ViewBNormal.xyz, _Normal.xyz);
    BumpNormal.xyz = mul(BumpNormal.xyz, matTBN);
    BumpNormal.w = 0.0f;

    return BumpNormal;
}