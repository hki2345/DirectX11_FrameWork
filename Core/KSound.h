#pragma once
#include "Resource.h"
#include "SoundDevice.h"

class KSound final : public Resource
{
private:
	friend class SoundPlayer;

private:
	FMOD::Sound* m_Sound;

public:
	bool Load();

public:
	KSound();
	~KSound();
};

