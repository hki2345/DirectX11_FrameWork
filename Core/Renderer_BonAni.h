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
		// �̸��� Begin -> string �Լ� ���� ������
		// ¥�� �°� �� ������ �ִµ�
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

	// �ϴ� ���� ���� ���ؼ� ���� �� �˾ƾ� �ϰڴ�.
	KPtr<MeshContainer> MCon;

	//FBXLoader* m_pLoader;

	// �ִϸ��̼� 0���� ������ ~ X ������ ���� �� �ð�
	int m_ClipInx;
	int m_FrameCnt;
	float m_UpdateSpd;
	float m_CurTime;
	float m_UpdateTime;

	KColor m_MeshColor;

	// ��İ� ���� ���� ������ �д�.
	std::vector<KMatrix> m_MXData_CurAni;
	std::vector<KMatrix> m_BoneData_CurAni;

	// ���� ������ ���̴��� �ѱ�⿣ ������ ũ�Ⱑ �ʹ� Ŀ����
	// ���� �ؽ��ķ� ������ ��� - �ؽ��Ĵ� �� ��ü�� ������ �� �� �ִ�.
	KPtr<Texture> m_pBoneTex;


public:
	// true - ����ƽ, false
	void Set_Static()
	{
		ROpt.IsBoneAni = 0;
	}
	void Set_Dynamic()
	{
		ROpt.IsBoneAni = 1;
	}

	// �ϴ�, ���� �ſ� ������ �����.

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

