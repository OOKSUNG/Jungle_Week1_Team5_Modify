#pragma once

#include "fmod.hpp"

enum class ESoundEffect
{
    ESE_Start,
    ESE_BGM,
    //ESE_End,

    ESE_Win,
    ESE_Lost,

    ESE_Shoot,
    ESE_Collide,
    ESE_Add,
    ESE_Pop,
    ESE_Fall,
    ESE_Bonus,

    ESE_Lightning,
    ESE_Bomb,

    ESE_Max
};

class SoundManager
{
public:
    static void CreateInstance();
    static void DeleteInstance();
    static SoundManager* GetInstance();

    void PlaySoundEffect(ESoundEffect soundEffect);
    void StopSoundEffect(ESoundEffect soundEffect);
    void StopAllSoundEffect();
    void Update();

private:
    SoundManager();
    ~SoundManager();

private:
    static SoundManager* Manager;
  
    enum
    {
        MAX_CHANNELS_COUNT = 32
    };

    FMOD::System* SoundSystem;
    FMOD::Sound* Sounds[static_cast<unsigned int>(ESoundEffect::ESE_Max)];
    
    // Todo: Can reuse channels
    FMOD::Channel* ChannelsOrNull[MAX_CHANNELS_COUNT];
};

