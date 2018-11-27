#pragma once
#include "Renderer.h"
#include "KFBX.h"
#include "KM3.h"


class Renderer_BonAni : public Renderer
{
private:
	class Ani_Changer : public Begin
	{
	public:
		// 이름은 Begin -> string 함수 쓰기 귀찮음
		// 짜피 걔가 다 가지고 있는뎅
		int Start;
		int End;
		float Speed;


	public:
		Ani_Changer(const wchar_t* _Name, const int& _Start, const int& _End)
			: Start (_Start), End(_End)
		{
			name(_Name);
		}
	};

public:
	Renderer_BonAni();
	~Renderer_BonAni();

private:
	std::map<std::wstring, KPtr<Ani_Changer>> m_ACMap;
	KPtr<Ani_Changer> m_CurAni;
	KPtr<Ani_Changer> m_NextAni;

	// 일단 본을 찍어내기 위해선 본을 다 알아야 하겠다.
	KPtr<MeshContainer> MCon;

	//FBXLoader* m_pLoader;

	// 애니메이션 0번쨰 프레임 ~ X 프레임 보는 그 시간
	int m_ClipInx;
	int m_FrameCnt;
	float m_UpdateSpd;
	float m_CurTime;
	float m_UpdateTime;

	KColor m_MeshColor;

	// 행렬과 본을 따로 저장해 둔다.
	std::vector<KMatrix> m_MXData_CurAni;
	std::vector<KMatrix> m_BoneData_CurAni;

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

private:
	void Init_Mesh();


public:
	KMatrix Get_BoneMX(const wchar_t* _Name);
	KMatrix Get_WBoneMX(const wchar_t* _Name);


	bool Erase_AniChanger(const wchar_t* _Name);
	bool Set_AniChanger(const wchar_t* _Name);
	KPtr<Ani_Changer> Create_AniChanger(const wchar_t* _Name, const int& _Start, const int& _End);
	KPtr<Ani_Changer> Find_AniChamnger(const wchar_t* _Name);

	void PrevUpdate_Ani();

public:
	void Load_FbxTest(const wchar_t* _Path);
	void Set_Fbx(const wchar_t* _Name);
	void Set_TexturePath(const TEX_TYPE& _Value, const wchar_t* _Path);
	void PrevUpdate() override;
	void RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx) override;

};

