#pragma once
#include "Renderer.h"
#include "KFBX.h"


class Renderer_BonAni : public Renderer
{

public:
	Renderer_BonAni();
	~Renderer_BonAni();

private:
	// �ϴ� ���� ���� ���ؼ� ���� �� �˾ƾ� �ϰڴ�.

	FBXLoader* m_pLoader;

	// �ִϸ��̼� 0���� ������ ~ X ������ ���� �� �ð�
	int m_ClipInx;
	int m_FrameCnt;
	float m_UpdateSpd;
	float m_CurTime;
	float m_UpdateTime;
	int SFrame;
	int EFrame;

public:
	std::vector<KMatrix> m_MXData_CurAni;

	// ���� ������ ���̴��� �ѱ�⿣ ������ ũ�Ⱑ �ʹ� Ŀ����
	// ���� �ؽ��ķ� ������ ��� - �ؽ��Ĵ� �� ��ü�� ������ �� �� �ִ�.
	KPtr<Texture> m_pBoneTex;

public:
	// �ϴ�, ���� �ſ� ������ �����.
	void Load_FbxTest(const wchar_t* _Path);
	void EndUpdate();
	void Render(KPtr<Camera> _Cam);

};

