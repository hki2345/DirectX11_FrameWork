#pragma once
#include <Component.h>

class Dlg_Component : public CDialogEx
{
public:
	UINT Y_SIZE;

public:
	virtual void set_component(Component* _Com) {};

public:
	Dlg_Component(int _id, CWnd* _pWnd) : CDialogEx(_id, _pWnd) {}
	~Dlg_Component() {};

	virtual BOOL OnInitDialog();
};

