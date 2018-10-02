#pragma once
#include "KResource.h"
#include "SoundDevice.h"

class KSound final : public KResource
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

