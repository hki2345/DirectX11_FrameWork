#pragma once

#ifdef X64
#pragma comment(lib, "fmod64_vc")
#else
#pragma comment(lib, "fmod_vc")
#endif

#include <fmod.hpp>

class HSoundDevice
{
public:
	friend class KCore;
	friend class HSound;
	friend class HSoundPlayer;

private:
	class StaticDestroy	{ 		friend HSoundDevice;		~StaticDestroy() { HSoundDevice::Destroy(); }		};
	friend StaticDestroy;
	static StaticDestroy Des;

private:
	static FMOD::System* SoundSystem;
	static void Init();
	static void Update();
	static void Destroy();


private:
	HSoundDevice();
	~HSoundDevice();
};

