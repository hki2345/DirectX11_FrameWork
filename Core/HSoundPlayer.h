#pragma once
#include "HComponent.h"
#include "HSound.h"

class HSoundPlayer : public HComponent
{
private:
	static FMOD_RESULT __stdcall SoundCallBack(FMOD_CHANNELCONTROL *channelcontrol
		, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void *commanddata1, void *commanddata2);

private:
	KPtr<HSound> m_Sound;
	FMOD::Channel* m_pChannel;
	bool m_bPlay;

private:
	void End();

public:
	bool SoundPlay(const wchar_t* _SoundName);

public:
	bool IsPlay() {
		return m_bPlay;
	}

public:
	HSoundPlayer();
	~HSoundPlayer();
};

