#pragma once
#include "TransPosition.h"

// �ǻ� ��� ���۳�Ʈ�� �ð��� ������ �޴´�.
#include "TimeManager.h"

// ���۳�Ʈ ������ �������
// �⺻������ Ʈ������ �������� - ���ù����ϰ� ����
// ���۳�Ʈ���� �ٷ� ����� �� �ְ� ��� �ִ� ���̴�.
class Component_DE : public Component
{
protected:
	TransPosition* m_Trans;

public:
	virtual void Init_Component() override;

public:
	Component_DE();
	~Component_DE();
};

