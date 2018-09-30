#pragma once


#ifdef WIN32
#pragma comment(lib, "fmod_vc")
#else
#pragma comment(lib, "fmod64_vc")
#endif

#include <fmod.hpp>

class SoundDevice
{
public:
	friend class KCore;
	friend class KSound;
	friend class SoundPlayer;

private:
	class StaticDestroyer 
	{
	public:
		friend SoundDevice;
		~StaticDestroyer() { SoundDevice::Destroy(); };
	};

	friend StaticDestroyer;
	static StaticDestroyer Destroyer;

private:
	static FMOD::System* SoundSystem;
	static void Init();
	static void Update();
	static void Destroy();

private:
	SoundDevice();
	~SoundDevice();
};

