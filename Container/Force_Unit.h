#pragma once
#include <SomeOne.h>
#include "Container.h"


class SC2_Force;
class Renderer_BonAni;
class KSphere_Col;
class Force_Unit : public SomeOne
{
	// ������ �Ӽ��� �����Ǵ� ����̱� ������ �� ������� �ϴ� �� �´�.
	// �� ��� - ��Ʈ���� ����
private:
	KPtr<KSphere_Col>		m_Col;
	KPtr<Renderer_BonAni>	m_Render;
	KPtr<SC2_Force> 		m_Force;

	float m_Speed;



public:
	virtual bool Init();
	virtual void Update();


public:
	Force_Unit(const wchar_t* _Name, KPtr<SC2_Force> _Force);
	~Force_Unit();
};

