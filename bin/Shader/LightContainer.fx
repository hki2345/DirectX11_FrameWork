#include "Matrix.fx"

class LightColor
{
	float4 Diff; // 확산광
	float4 Spec; // 반사광
	float4 Ambi; // 환경광
};
class LightData
{
	LightColor Lc;
	float4 Pos;
	float4 Dir;
	float4 CamPos;
	float Range;
	float Angle;
	int Type;
};

cbuffer LightBuffer : register(b10)
{
	LightData ArrList[10];
	int       LightCount;
	int       Temp1;
	int       Temp2;
}


struct LIGHT_VT_IN
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

struct LIGHT_VT_OUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vViewPos : POSITION;
	float4 vNormal : NORMAL;
};

struct LIGHT_PX_OUT
{
	float4 vColor : SV_Target;
};


LightColor Direct_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
	LightColor CALL = (LightColor)0.f;

	// 뷰포트를 빼면 안됀다 -> 이건 다이렉트 -> 고정형 조명이다
	float4 vLight = _Info.Dir;
	vLight = normalize(vLight);
	CALL.Diff = _Info.Lc.Diff * saturate(dot(_vViewNormal, vLight));

	// 반사광 -> 카메라 각도와 그 법선벡터을 이용 -> 튕겨나가는 벡터를 구함 -> 정규화
	float4 vReflect = normalize(2.f * dot(vLight, _vViewNormal) * _vViewNormal - vLight);
	float4 vEye = -normalize(_vViewPos);


	CALL.Spec = _Info.Lc.Spec * pow(saturate(dot(vEye, vReflect)), 10);
	CALL.Ambi = _Info.Lc.Ambi;

	return CALL;
}


LightColor Point_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
	LightColor CALL = (LightColor)0.f;

	// 포인트는 단순 포지션만 고려 - 방향은 따로 없음
	// 멀을 해주는 이유는 -> 월드 행렬 좌표이기 때문에 ㅇㅇ
	// 이거... 멀도 필요없고 그냥 g_WVP로 연산된 객체좌표가 필요함 -> 빛의 위치에서 빠져야하는 부분이니까
	// 근데 월드포스 마지막에 1들어가서 카메라랑 같이움직이는데 어찌할가.
	matrix TempMat = g_W * g_V;

	float4 vLight = normalize(
		float4(_Info.Pos.x, _Info.Pos.y, _Info.Pos.z, .0f) -
		float4(TempMat._41, TempMat._42, TempMat._43, .0f));

	// Range
	CALL.Diff = _Info.Lc.Diff * saturate(dot(_vViewNormal, vLight));

	// 반사광 -> 카메라 각도와 그 법선벡터을 이용 -> 튕겨나가는 벡터를 구함 -> 정규화
	float4 vReflect = normalize(2.f * dot(vLight, _vViewNormal) * _vViewNormal - vLight);
	float4 vEye = -normalize(_vViewPos);

	// float4 vEye = normalize(
	// 	float4(_Info.CamPos.x, _Info.CamPos.y, _Info.CamPos.z, .0f) -
	// 	float4(TempMat._41, TempMat._42, TempMat._43, .0f));


	CALL.Spec = _Info.Lc.Spec * pow(saturate(dot(vEye, vReflect)), 10);
	CALL.Ambi = _Info.Lc.Ambi;

	return CALL;
}