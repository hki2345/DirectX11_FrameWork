#pragma once
#include <DXContainer.h>
#include <list>


class Force_Container : public SmartPtr
{
public:
	wchar_t Name[NAMENUM];
	KColor Color;

public:
	Force_Container();
	Force_Container(const Force_Container& _Other);
	~Force_Container() {};
};

enum ZERGUNIT_FORM
{
	ZF_STAND001 = 0,
	ZF_STAND002,
	ZF_STAND003,
	ZF_WALK01,
	ZF_WALK02,
	ZF_ATTACK,
	ZF_ATTACK_COVER01,
	ZF_ATTACK_COVER02,
	ZF_BURROW,
	ZF_UNBURROW,
	ZF_FIDGET01,
	ZF_FIDGET02,
};

enum TERRANUNIT_FORM
{
	TF_STAND001 = 0,
	TF_STAND002,
	TF_STAND003,
	TF_WALK01,
	TF_WALK02,
	TF_SPELL01,
	TF_SPELL02,
	TF_SPELL03,
	TF_ATTACK,
	TF_ATTACK_COVER01,
	TF_ATTACK_COVER02,
	TF_FIDGET01,
	TF_FIDGET02,
};