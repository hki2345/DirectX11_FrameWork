#pragma once
#include <SomeOne.h>
#include "Container.h"


class SC2_Force;
class Renderer_BonAni;
class KSphere_Col;
class Force_Unit : public SomeOne
{
	// 유닛의 속성은 누적되는 방식이기 때문에 이 방식으로 하는 게 맞다.
	// 이 방식 - 비트단위 연산
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

