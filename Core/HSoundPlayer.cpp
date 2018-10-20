#include "HSoundPlayer.h"
#include "HResMgr.h"
#include "KMacro.h"



FMOD_RESULT __stdcall HSoundPlayer::SoundCallBack(FMOD_CHANNELCONTROL *channelcontrol
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

		HSoundPlayer* Ptr = nullptr;
		CallPtr->getUserData((void**)(&Ptr));

		switch (callbacktype)
		{
		case FMOD_CHANNELCONTROL_CALLBACK_END:
		{
			Ptr->End();
			break;
		}
		default:
			break;
		}
	}

	int a = 0;

	return FMOD_OK;
}

void HSoundPlayer::End() 
{
	m_bPlay = false;
	m_pChannel = nullptr;
}

HSoundPlayer::HSoundPlayer() : m_pChannel(nullptr), m_Sound(nullptr), m_bPlay(false)
{
	//if (nullptr != m_pChannel)
	//{
	//}

}


HSoundPlayer::~HSoundPlayer()
{
}

bool HSoundPlayer::SoundPlay(const wchar_t* _SoundName)
{
	m_Sound = ResourceManager<HSound>::Find(_SoundName);

	KASSERT(nullptr == m_Sound);

	FMOD_RESULT FR = HSoundDevice::SoundSystem->playSound(m_Sound->m_pSound, nullptr, false, &m_pChannel);

	m_pChannel->setCallback(SoundCallBack);
	m_pChannel->setUserData(this);

	// void* ptr;

	// HSoundDevice::SoundSystem->getUserData(&ptr);
	// m_pChannel->getUserData()

	m_bPlay = true;

	if (FMOD_OK != FR)
	{
		KASSERT(true);
	}

	return true;
}