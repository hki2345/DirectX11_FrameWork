#include "KSound.h"
#include "KMacro.h"
#include <atlstr.h>


KSound::KSound() : m_Sound(nullptr)
{
}


KSound::~KSound()
{
	if (nullptr != SoundDevice::SoundSystem)
	{
		if (nullptr != m_Sound)
		{
			m_Sound->release();
		}
	}
}

bool KSound::Load()
{
	// 인코딩을 변경해줘야 한다. UTF-8 문자열만 받는다,.
	// CP_UTF8안해주면 안됐다고!!!!!!!!!! -> 는 또 됏나보군 ㅇㅇ
	std::string Str = CW2A(path(), CP_UTF8);

	if (FMOD_OK != SoundDevice::SoundSystem->createSound(Str.c_str(),
		FMOD_DEFAULT, nullptr, &m_Sound))
	{
		KASSERT(true);
		return false;
	}

	KASSERT(nullptr == m_Sound);

	if (nullptr == m_Sound)
	{
		return false;
	}

	return true;

}