#pragma once
#include "HBaseCom.h"

// 여러분들이 상속받아서 써야할 클래스
class HLogic : public HBaseCom
{
public:
	HLogic();
	virtual ~HLogic() = 0;
};

