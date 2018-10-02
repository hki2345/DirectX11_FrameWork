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

// 바로 텍스쳐의 색으 ㄹ얻는 것
float4 Get_Color(Texture2D _Tex, int _Smp, float2 _Uv)
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
            return (float4) 0.0f;
    }
}

// 텍스처에 샘플러 연산까지 전부 끝 맞힌 값
float4 Get_CalColor(int _Tex, int _Smp, float2 _Uv)
{
    switch (_Tex)
    {
        case 0:
            return Get_Color(g_Tex_0, _Smp, _Uv);
        case 1:
            return Get_Color(g_Tex_1, _Smp, _Uv);
        case 2:
            return Get_Color(g_Tex_2, _Smp, _Uv);
        case 3:
            return Get_Color(g_Tex_3, _Smp, _Uv);
        case 4:
            return Get_Color(g_Tex_4, _Smp, _Uv);
        case 5:
            return Get_Color(g_Tex_5, _Smp, _Uv);
        case 6:
            return Get_Color(g_Tex_6, _Smp, _Uv);
        case 7:
            return Get_Color(g_Tex_7, _Smp, _Uv);
        default:
            return (float4) 0.0f;
    }
}