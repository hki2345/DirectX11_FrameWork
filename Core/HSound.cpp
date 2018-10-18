#include "HSound.h"
#include "KMacro.h"
#include <atlstr.h>

HSound::HSound() : m_pSound(nullptr)
{
}


HSound::~HSound()
{
	if (nullptr != HSoundDevice::SoundSystem)
	{
		if (nullptr != m_pSound)
		{
			m_pSound->release();
		}
	}
}

bool HSound::Load() 
{
	// ���ڵ��� ��������� �Ѵ�. UTF-8 ���ڿ��� �޴´�,.
	// CP_UTF8�����ָ� �ȵƴٰ�!!!!!!!!!!
	std::string Str = CW2A(Path(), CP_UTF8);

	if (FMOD_OK != HSoundDevice::SoundSystem->createSound(Str.c_str(), 
		FMOD_DEFAULT, nullptr, &m_pSound))
	{
		KASSERT(true);
		return false;
	}

	KASSERT(nullptr == m_pSound);

	if (nullptr == m_pSound)
	{
		return false;
	}

	return true;

}