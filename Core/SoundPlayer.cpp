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
			// �뷡�� ������.
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

// ���� ����� �����ٸ� �ȴ� - ä���� �ʱ�ȭ ���� ���� - �ٽ� �� �� ����
void SoundPlayer::Loop_End()
{
	m_bPlay = false;
}

// ���� ��Ű�� �༮ -> �뷡�� �������� �̳༮�� ������ �ƴϴ�.
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

bool SoundPlayer::Set_Sound(const wchar_t* _SoundName, const float& _Volume /*= 1.0f*/)
{
	m_Sound = ResourceManager<Sound>::Find(_SoundName);

	KASSERT(nullptr == m_Sound);

	name(_SoundName);
	m_Volume = _Volume;

	return true;
}

bool SoundPlayer::Play(const wchar_t* _SoundName, const float& _Volume/* = 1.0f*/)
{
	m_Sound = ResourceManager<Sound>::Find(_SoundName);

	KASSERT(nullptr == m_Sound);

	name(_SoundName);

	FMOD_RESULT FR = SoundDevice::SoundSystem->playSound(m_Sound->m_pSound, nullptr, false, &m_Channel);

	m_Channel->setCallback(SoundCallBack);
	m_Channel->setUserData(this);


	m_Volume = _Volume;
	m_Channel->setVolume(m_Volume);

	m_bPlay = true;

	if (FMOD_OK != FR)
	{
		KASSERT(true);
	}

	return true;
}


bool SoundPlayer::Play()
{
	FMOD_RESULT FR = SoundDevice::SoundSystem->playSound(m_Sound->m_pSound, nullptr, false, &m_Channel);

	m_Channel->setCallback(SoundCallBack);
	m_Channel->setUserData(this);
	m_Channel->setVolume(m_Volume);

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