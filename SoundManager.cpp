
#include <cassert>

#include "SoundManager.h"
#include <string>

SoundManager* SoundManager::Manager = nullptr;

void SoundManager::CreateInstance()
{
    assert(Manager == nullptr);

    Manager = new SoundManager();
}

void SoundManager::DeleteInstance()
{
    assert(Manager != nullptr);

    delete Manager;
    Manager = nullptr;
}

SoundManager* SoundManager::GetInstance()
{
    assert(Manager != nullptr);

    return Manager;
}

SoundManager::SoundManager()
	: SoundSystem(nullptr)
{
	const unsigned int MAX_SOUNDS_COUNT = static_cast<unsigned int>(ESoundEffect::ESE_Max);

	for (unsigned int i = 0; i < MAX_SOUNDS_COUNT; ++i)
	{
		Sounds[i] = nullptr;
		Channels[i] = nullptr;
	}

	FMOD_RESULT soundResult;
	void* extraDriverData(nullptr);

	soundResult = FMOD::System_Create(&SoundSystem);
	if (soundResult != FMOD_OK)
	{
		assert(false);
	}

	soundResult = SoundSystem->init(MAX_SOUNDS_COUNT, FMOD_INIT_NORMAL, extraDriverData);
	if (soundResult != FMOD_OK)
	{
		assert(false);
	}

	// Todo: Create all sounds
	std::string soundStrings[MAX_SOUNDS_COUNT] =
	{
		//"Resources/Sounds/Start.mp3",
		"Resources/Sounds/BGM.mp3",
		//"Resources/Sounds/End.mp3",
		"Resources/Sounds/Win.mp3",
		"Resources/Sounds/Lost.mp3",
		"Resources/Sounds/Shoot.mp3",
		"Resources/Sounds/Collide.mp3",
		"Resources/Sounds/Add.mp3",
		"Resources/Sounds/Pop.mp3",
		"Resources/Sounds/Fall.mp3",
		"Resources/Sounds/Bonus.mp3"
	};
	
	for (unsigned int soundsIndex = 0; soundsIndex < MAX_SOUNDS_COUNT; ++soundsIndex)
	{
		soundResult = SoundSystem->createSound(soundStrings[soundsIndex].c_str(), FMOD_LOOP_OFF, 0, &Sounds[soundsIndex]);
	}
}

SoundManager::~SoundManager()
{
	for (unsigned int soundsIndex = 0; soundsIndex < static_cast<unsigned int>(ESoundEffect::ESE_Max); ++soundsIndex)
	{
		Sounds[soundsIndex]->release();
	}

    SoundSystem->release();
}

void SoundManager::PlaySoundEffect(ESoundEffect soundEffect)
{
	unsigned int playIndex = static_cast<unsigned int>(soundEffect);
	assert(Sounds[playIndex] != nullptr);

	FMOD_RESULT soundResult = SoundSystem->playSound(Sounds[playIndex], 0, false, &Channels[playIndex]);
}

void SoundManager::Update()
{
	FMOD_RESULT soundResult = SoundSystem->update();
}
