#pragma once
#include "Renderer.h"
#include "KFBX.h"
#include "KM3.h"
#include "Changer_Animation.h"

class Renderer_BonAni : public Renderer
{
public:
	Renderer_BonAni();
	~Renderer_BonAni();

private:
	// �ϴ� ���� ���� ���ؼ� ���� �� �˾ƾ� �ϰڴ�.
	KPtr<MeshContainer> MCon;
	KPtr<Changer_Animation> CAni;

	//FBXLoader* m_pLoader;

	// �ִϸ��̼� 0���� ������ ~ X ������ ���� �� �ð�
	int m_ClipInx;
	int m_FrameCnt;
	float m_UpdateSpd;
	float m_CurTime;
	float m_UpdateTime;

	KColor m_ForceColor;

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

	KColor& force_color()
	{
		return m_ForceColor;
	}
	void force_color(const KColor& _Value)
	{
		m_ForceColor = _Value;
	}

	// �ϴ�, ���� �ſ� ������ �����.

private:
	void Init_Mesh();


public:
	KPtr<Changer_Animation> changer_animation()
	{
		return CAni;
	}
	KMatrix Get_BoneMX(const wchar_t* _Name);
	KMatrix Get_WBoneMX(const wchar_t* _Name);


	void PrevUpdate_Ani();

public:
	void Load_FbxTest(const wchar_t* _Path);
	void Set_Fbx(const wchar_t* _Name);
	void Set_TexturePath(const TEX_TYPE& _Value, const wchar_t* _Path);
	void PrevUpdate() override;
	void RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx) override;


	// ���������� ��Ű�� �ڵ����� �����Ǽ� ����.
	// ���� ���� �������� ���� �ִ�.
	KPtr<Changer_Animation> Create_Animation();


	KPtr<Changer_Animation::Ani_Clip> Create_Clip(const wchar_t* _Name, const int& _Start, const int& _End);
	bool Erase_Clip(const wchar_t* _Name);
	void Set_Clip(const wchar_t* _Name);
	void Set_Clip(const int& _Num);
};

