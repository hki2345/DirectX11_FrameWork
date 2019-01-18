#include "SoundPlayer.h"
#include "ResourceManager.h"
#include "KMacro.h"


bool SoundPlayer::m_Loop = false;

SoundPlayer::SoundPlayer() : m_Channel(nullptr), m_Sound(nullptr), m_bPlay(false)
{
}


SoundPlayer::~SoundPlayer()
{
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
			// 노래가 끝났다.
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

void SoundPlayer::End() 
{
	m_bPlay = false;
	m_Channel = nullptr;
}

// 루프 엔드는 끝났다만 안다 - 채널이 초기화 되지 않음 - 다시 켤 수 있음
void SoundPlayer::Loop_End()
{
	m_bPlay = false;
}

// 루프 시키는 녀석 -> 노래가 끝났지만 이녀석은 루프가 아니다.
void SoundPlayer::Update()
{
	if (true == m_Loop)
	{
		if (false == m_bPlay && nullptr != m_Channel)
		{
			Play(name());
		}
	}
}

bool SoundPlayer::Play(const wchar_t* _SoundName, const float& _Volume/* = 1.0f*/)
{
	m_Sound = ResourceManager<Sound>::Find(_SoundName);

	KASSERT(nullptr == m_Sound);

	name(_SoundName);

	FMOD_RESULT FR = SoundDevice::SoundSystem->playSound(m_Sound->m_pSound, nullptr, false, &m_Channel);

	m_Channel->setCallback(SoundCallBack);
	m_Channel->setUserData(this);

	m_Channel->setVolume(_Volume);

	m_bPlay = true;

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
		FMOD_RESULT FR = SoundDevice::SoundSystem->playSound(m_Sound->m_pSound, nullptr, false, &m_Channel);
	}

	if (FMOD_OK != m_Channel->stop())
	{
		return false;
	}

	End();
	return true;
}