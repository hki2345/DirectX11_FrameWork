#include "SoundPlayer.h"
#include "ResourceManager.h"
#include "KMACRO.h"


bool SoundPlayer::m_Loop = false;


SoundPlayer::SoundPlayer() : m_Channel(nullptr), m_Sound(nullptr), m_IsPlay(false)
{
}


SoundPlayer::~SoundPlayer()
{
}




void SoundPlayer::Update()
{
	if (true == m_Loop)
	{
		if (false == m_IsPlay && nullptr != m_Channel)
		{
			Play(m_Name.c_str());
		}
	}
}

bool SoundPlayer::Play(const wchar_t* _SoundName)
{
	m_Sound = ResourceManager<KSound>::Find(_SoundName);

	KASSERT(nullptr == m_Sound);

	m_Name = _SoundName;

	FMOD_RESULT FR = SoundDevice::SoundSystem->playSound(m_Sound->m_Sound, nullptr, false, &m_Channel);

	m_Channel->setCallback(SoundCallBack);
	m_Channel->setUserData(this);

	m_IsPlay = true;

	if (FMOD_OK != FR)
	{
		KASSERT(true);
	}

	return true;
}


bool SoundPlayer::Stop()
{
	if (nullptr == m_Channel)
	{
		FMOD_RESULT FR = SoundDevice::SoundSystem->playSound(m_Sound->m_Sound, nullptr, false, &m_Channel);
	}

	if (FMOD_OK != m_Channel->stop())
	{
		return false;
	}

	End();
	return true;
}



void SoundPlayer::End()
{
	m_IsPlay = false;
	m_Channel = nullptr;
}

void SoundPlayer::Loop_End()
{
	m_IsPlay = false;
}


FMOD_RESULT __stdcall SoundPlayer::SoundCallBack(FMOD_CHANNELCONTROL *channelcontrol
	, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void *commanddata1, void *commanddata2)
{
	// channelcontrol

	// FMOD_CHANNELCONTROL_CHANNEL,
	//FMOD_CHANNELCONTROL_CHANNELGROUP,
	//FMOD_CHANNELCONTROL_FORCEINT = 65536    /* Makes sure this enum is signed 32bit. */

	if (controltype == FMOD_CHANNELCONTROL_CHANNEL)
	{
		FMOD::Channel* CallPtr = (FMOD::Channel*)channelcontrol;

		SoundPlayer* Ptr = nullptr;
		CallPtr->getUserData((void**)(&Ptr));

		switch (callbacktype)
		{
		case FMOD_CHANNELCONTROL_CALLBACK_END:
		{
			if (true == m_Loop)
			{
				Ptr->Loop_End();
			}
			else
			{
				Ptr->End();
			}
			break;
		}
		default:
			break;
		}
	}

	return FMOD_OK;
}