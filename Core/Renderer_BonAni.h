#pragma once
#include "Renderer.h"
#include "KFBX.h"


class Renderer_BonAni : public Renderer
{

public:
	Renderer_BonAni();
	~Renderer_BonAni();

private:
	// 일단 본을 찍어내기 위해선 본을 다 알아야 하겠다.

	FBXLoader* m_pLoader;

	// 애니메이션 0번쨰 프레임 ~ X 프레임 보는 그 시간
	int m_ClipInx;
	int m_FrameCnt;
	float m_UpdateSpd;
	float m_CurTime;
	float m_UpdateTime;
	int SFrame;
	int EFrame;

public:
	std::vector<KMatrix> m_MXData_CurAni;

	// 본의 정보가 쉐이더로 넘기기엔 버퍼의 크기가 너무 커지니
	// 차라리 텍스쳐로 보내는 방법 - 텍스쳐는 그 자체로 정보가 될 수 있다.
	KPtr<Texture> m_pBoneTex;

public:
	// true - 스테틱, false
	void Set_Static()
	{
		ROpt.IsBoneAni = 0;
	}
	void Set_Dynamic()
	{
		ROpt.IsBoneAni = 1;
	}

	// 일단, 듸우는 거에 초점을 맞춘다.
	void Load_FbxTest(const wchar_t* _Path);
	void EndUpdate();
	void Render(KPtr<Camera> _Cam);

};

