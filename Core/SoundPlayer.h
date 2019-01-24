#pragma once
#include "Component.h"
#include "Sound.h"

class SoundPlayer : public Component
{

public:
	SoundPlayer();
	~SoundPlayer();

private:
	static FMOD_RESULT __stdcall SoundCallBack(FMOD_CHANNELCONTROL *channelcontrol
		, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void *commanddata1, void *commanddata2);

private:
	static bool m_Loop;
	KPtr<Sound> m_Sound;
	FMOD::Channel* m_Channel;
	float m_Volume;

	bool m_bPlay;

private:
	void End();
	void Loop_End();

public:
	void Update() override;

	bool Set_Sound(const wchar_t* _SoundName, const float& _Volume = 1.0f);
	bool Play(const wchar_t* _SoundName , const float& _Volume = 1.0f);
	bool Play();
	bool Stop();

public:
	bool IsPlay() {
		return m_bPlay;
	}

	void Loop()
	{
		m_Loop = true;
	}

	void UnLoop()
	{
		m_Loop = false;
	}

};

