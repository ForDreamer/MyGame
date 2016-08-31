#ifndef _FmodSound_H_
#define _FmodSound_H_

#include "fmod.h"
#include "fmod_errors.h"
#include "fmod.hpp"
enum SoundChannelIndex
{
	SND_SELECT = 0,
	SND_CONFIRM = 1,
	SND_OPENDOOR = 2,
	SND_INTOENTRY = 3,
	SND_FALL = 4,
	SND_COIN = 5,
	SND_INN = 6,
	SND_GOTOTANK = 7,
	SND_GETITEM = 8,
	SND_STARTBATTLE = 9,
	SND_ENEMYAPPEAR = 10,
	SND_ESCAPE = 11,
	SND_FAIL = 12,
	SND_WINNING = 13,
	SND_DISAPPEAR = 14,
	SND_LEVELUP = 15,

	SND_WT_BODY = 16,
	SND_WT_BODY_END = 17,
	SND_WT_CANNON = 18,
	SND_WT_CANNON_END = 19,
	SND_WT_FIRE = 20,
	SND_WT_FIRE_END = 21,
	SND_WT_SLING = 22,
	SND_WT_SLING_END = 23,
	SND_WT_GUN = 24,

	MAX_SOUND_CHANNELS
};

enum MusicIndex
{
	BGM_STARTUP = 0,
	BGM_NAMESETTING = 1,
	BGM_TOWNTHEME = 2,
	BGM_WORLDTHEME = 3,
	BGM_CAVETHEME = 4,
	BGM_GATETHEME = 5,
	BGM_BARTHEME = 6,
	BGM_TANKTHEME = 7,
	BGM_BATTLETHEME = 8,
	BGM_BOSSFIGHT = 9,

	MAX_MUSIC_INDEX
};

#define MUSIC_CHANNEL MAX_SOUND_CHANNELS
#define MAX_CHANNELS MAX_SOUND_CHANNELS+1

struct FmodSound
{
public:
	static bool InitFmodSound(void);
	static void CloseFmodSound(void);

	static void PlaySnd(unsigned int soundIndex);
	static void StopSnd(unsigned int soundIndex);

	static void PlayMusic(unsigned int musicIndex);
	static void StopMusic(void);
	static bool IsMusicStopped(void);

private:
	static bool LoadSoundSample(unsigned int soundIndex);

private:
	static FMOD::System* system;
	static FMOD_RESULT result;
	static FMOD::Sound* spSoundSample[MAX_SOUND_CHANNELS];
	static FMOD::Sound* spSoundStream;
	static int sLastMusicIndex;
};

#endif