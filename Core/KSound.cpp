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
	// ���ڵ��� ��������� �Ѵ�. UTF-8 ���ڿ��� �޴´�,.
	// CP_UTF8�����ָ� �ȵƴٰ�!!!!!!!!!! -> �� �� �ѳ����� ����
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