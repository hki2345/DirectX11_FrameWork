#pragma once
#include "Component.h"
#include "KSound.h"

class SoundPlayer : public Component
{
private:
	static FMOD_RESULT __stdcall SoundCallBack(FMOD_CHANNELCONTROL *channelcontrol
		, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void *commanddata1, void *commanddata2);

private:
	static bool m_Loop;
	KPtr<KSound> m_Sound;
	FMOD::Channel* m_Channel;
	bool m_IsPlay;
	std::wstring m_Name;


private:
	void End();
	void Loop_End();

public:
	void Update() override;

	bool Play(const wchar_t* _SoundName);
	bool Stop();

	bool IsPlay() {
		return m_IsPlay;
	}

	void Loop()
	{
		m_Loop = true;
	}
	
	void UnLoop()
	{
		m_Loop = false;
	}

public:
	SoundPlayer();
	~SoundPlayer();
};

