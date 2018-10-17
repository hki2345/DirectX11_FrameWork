#include "HSoundDevice.h"
#include "HMACRO.h"

FMOD::System* HSoundDevice::SoundSystem = nullptr;

HSoundDevice::StaticDestroy HSoundDevice::Des;

void HSoundDevice::Destroy() 
{
	if (nullptr != HSoundDevice::SoundSystem)
	{
		HSoundDevice::SoundSystem->release();
		HSoundDevice::SoundSystem = nullptr;
	}
}

HSoundDevice::HSoundDevice()
{
}


HSoundDevice::~HSoundDevice()
{
}

void HSoundDevice::Init() 
{
	// �������� �ϴ���?
	FMOD::System_Create(&SoundSystem);

	TASSERT(nullptr == SoundSystem);

	if (nullptr == SoundSystem)
	{
		return;
	}

	// ��ü�� ���Դ�.
	// �׷��� �׻� �˻��ض�.
	// Release
	// Delete
	// Create
	// Init�� ���� �ý����� �غ��ϴ� �Լ��̴�.
	// 1��° ���� ���ÿ� � ���带 ���???
	// 2��° � ������� ���鷹. 0������ �׳� ����ϰ�
	// 3��° ����  ���� ��ġ ���ö� Ư���ϰ� �����������Ұ� �ֳ�
	// ��ġ�� �������.
	FMOD_RESULT FR = SoundSystem->init(32, FMOD_DEFAULT, nullptr);

	TASSERT(FMOD_OK != FR);

	if (FMOD_OK != FR)
	{
		return;
	}
}

void HSoundDevice::Update() 
{
	if (nullptr != SoundSystem)
	{
		// �ݹ��Լ� ȣ���� ����������.
		SoundSystem->update();
	}
}