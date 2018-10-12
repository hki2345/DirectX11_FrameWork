
// 거진 쉐이더파일 모든 곳에서 필요한 것
cbuffer TransformBuffer : register(b1)
{
	matrix g_W;
	matrix g_V;
	matrix g_P;
    matrix g_WV;
	matrix g_WVP;
}

class Texture_Data
{
    int TInx;
    int TSmp;
    int ESmp;
    int Type;
};

cbuffer RenderOption : register(b11)
{
    int CheckLight;
    int Deffert_orFoward;
    int VzPo; // Vert -> 0, Pixel -> 1
    int TexCnt;
    Texture_Data Texes[8]; // 20 216
}