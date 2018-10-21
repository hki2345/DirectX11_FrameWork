#pragma once
#include "Component_DE.h"

// 여러분들이 상속받아서 써야할 클래스
class SomeOne : public Component_DE
{
public:
	SomeOne();
	virtual ~SomeOne() = 0;
};

