#pragma once
#include "Resource.h"
#include "HSoundDevice.h"

class HSound final : public Resource
{
private:
	friend class HSoundPlayer;

private:
	FMOD::Sound* m_pSound;

public:
	bool Load();


public:
	HSound();
	~HSound();
};

