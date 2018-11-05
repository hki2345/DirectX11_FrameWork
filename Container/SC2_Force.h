#pragma once
#include <Begin.h>
#include <Begin_Updater.h>
#include <DXContainer.h>

class SC2_Force : public Begin_Updater
{
	enum PLAYER_TYPE
	{
		PRT_NONE,
		PRT_PC,
		PRT_PERSON,
	};

private:
	KColor m_Color;



public:
	SC2_Force();
	~SC2_Force();
};

