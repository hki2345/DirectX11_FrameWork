#pragma once
#include "HRes.h"
#include "HSoundDevice.h"

class HSound final : public HRes
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

