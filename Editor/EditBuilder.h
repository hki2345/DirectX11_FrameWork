#pragma once
#include <State.h>


class EditBuilder : public State::State_Builder
{
public:
	EditBuilder();
	~EditBuilder();


	// State_Builder��(��) ���� ��ӵ�
	virtual void Build_State() override {};
};

