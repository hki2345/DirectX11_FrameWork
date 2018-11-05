#pragma once
#include "Renderer.h"
#include "KFBX.h"
#include "KM3.h"


class Renderer_BonAni : public Renderer
{

public:
	Renderer_BonAni();
	~Renderer_BonAni();

private:
	// �ϴ� ���� ���� ���ؼ� ���� �� �˾ƾ� �ϰڴ�.
	KPtr<MeshContainer> MCon;

	//FBXLoader* m_pLoader;

	// �ִϸ��̼� 0���� ������ ~ X ������ ���� �� �ð�
	int m_ClipInx;
	int m_FrameCnt;
	float m_UpdateSpd;
	float m_CurTime;
	float m_UpdateTime;
	int SFrame;
	int EFrame;

	KColor m_MeshColor;

public:

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

public:
	void Load_FbxTest(const wchar_t* _Path);
	void Set_Fbx(const wchar_t* _Name);
	void PrevUpdate() override;
	void Render(KPtr<Camera> _Cam);

};

