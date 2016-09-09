#include "fmodsound.h"
#include <windows.h>
#include <strsafe.h>

FMOD::System* FmodSound::system = NULL;
FMOD_RESULT FmodSound::result = FMOD_OK;
FMOD::Sound* FmodSound::spSoundSample[MAX_SOUND_CHANNELS] = { NULL };
FMOD::Sound* FmodSound::spSoundStream = NULL;
int FmodSound::sLastMusicIndex = -1;

static const char* sSoundFileNames[MAX_SOUND_CHANNELS] =
{
	"../Sound/Sfx/Select.wav",
	"../Sound/Sfx/Confirm.wav",
	"../Sound/Sfx/OpenDoor.wav",
	"../Sound/Sfx/IntoEntry.wav",
	"../Sound/Sfx/Fall.wav",
	"../Sound/Sfx/Coin.wav",
	"../Sound/Sfx/Inn.mp3",
	"../Sound/Sfx/GotoTank.wav",
	"../Sound/Sfx/GetItem.wav",
	"../Sound/Sfx/StartBattle.mp3",
	"../Sound/Sfx/EnemyAppear.wav",
	"../Sound/Sfx/Escape.wav",
	"../Sound/Sfx/Fail.mp3",
	"../Sound/Sfx/Winning.mp3",
	"../Sound/Sfx/Disappear.wav",
	"../Sound/Sfx/LevelUp.mp3",
	"../Sound/Sfx/WT_Body.wav",
	"../Sound/Sfx/WT_Body_End.wav",
	"../Sound/Sfx/WT_Cannon.wav",
	"../Sound/Sfx/WT_Cannon_End.wav",
	"../Sound/Sfx/WT_Fire.wav",
	"../Sound/Sfx/WT_Fire_End.wav",
	"../Sound/Sfx/WT_Sling.wav",
	"../Sound/Sfx/WT_Sling_End.wav",
	"../Sound/Sfx/WT_Gun.wav"
};

static const char* sMusicFileNames[MAX_MUSIC_INDEX] =
{
	"../Sound/Bgm/Startup.mp3",
	"../Sound/Bgm/NameSetting.mp3",
	"../Sound/Bgm/TownTheme.mp3",
	"../Sound/Bgm/WorldTheme.mp3",
	"../Sound/Bgm/CaveTheme.mp3",
	"../Sound/Bgm/GateTheme.mp3",
	"../Sound/Bgm/BarTheme.mp3",
	"../Sound/Bgm/TankTheme.mp3",
	"../Sound/Bgm/BattleTheme.mp3",
	"../Sound/Bgm/BossFight.mp3"
};

/************************************************************************/
/* 初始化FMOD声音系统                                                     */
/************************************************************************/
bool FmodSound::InitFmodSound(void)
{
	
	FMOD_RESULT result;
	unsigned int      version;
	
	result = FMOD::System_Create(&system);
	FMOD_ErrorString(result);

	result = system->getVersion(&version);
	FMOD_ErrorString(result);

	result = system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
	FMOD_ErrorString(result);
// 	if(!FSOUND_Init(44100, MAX_CHANNELS, FSOUND_INIT_GLOBALFOCUS))
// 	{
// 		MessageBox(0, (LPCWSTR)"Fmod FSOUND_Init() - FAILED", 0, 0);
// 		return false;
// 	}
	
	for(int i=0; i<MAX_SOUND_CHANNELS; i++)
	{
		result = system->createSound(sSoundFileNames[i], FMOD_DEFAULT, 0, &spSoundSample[i]);
		FMOD_ErrorString(result);
// 		if(!LoadSoundSample(i))
// 			return false;
	}

	return true;
}
	
void FmodSound::CloseFmodSound(void)
{
	for(int i=0; i<MAX_SOUND_CHANNELS; i++)
	{
		result = spSoundSample[i]->release();
		//FSOUND_Sample_Free(spSoundSample[i]);
	}
	result = system->close();
	result = system->release();
// 	FSOUND_Stream_Close(spSoundStream);
// 	FSOUND_Close();
}

// bool FmodSound::LoadSoundSample(unsigned int soundIndex)
// {
// 
// 	spSoundSample[soundIndex] = FSOUND_Sample_Load(soundIndex, sSoundFileNames[soundIndex], 0, 0);
// 	if(spSoundSample[soundIndex])
// 	{
// 		FSOUND_Sample_SetMode(spSoundSample[soundIndex], FSOUND_LOOP_OFF|FSOUND_NORMAL);
// 	}
// 	else
// 	{
// 		char errStr[128];
// 		sprintf_s(errStr, "Failed to load sound file: %s", sSoundFileNames[soundIndex]);
// 		MessageBox(0, (LPCWSTR)errStr, 0, 0);
// 		return false;
// 	}
// 
// 	return true;
// }

void FmodSound::PlaySnd(unsigned int soundIndex)
{
	
	FMOD::Channel    *channel = 0;
	system->playSound(spSoundSample[soundIndex], 0, false, &channel);
	FMOD_ErrorString(result);

	result = system->update();
	FMOD_ErrorString(result);
// 	FSOUND_PlaySound(soundIndex, spSoundSample[soundIndex]);
}

void FmodSound::StopSnd(unsigned int soundIndex)
{
	FMOD::Channel    *channel = 0;
	system->getChannel(soundIndex, &channel);
	channel->stop();
	//FSOUND_StopSound(soundIndex);
}

void FmodSound::PlayMusic(unsigned int musicIndex)
{
	if(sLastMusicIndex == musicIndex)
		return;
	FMOD::Channel    *channel = 0;
	//FSOUND_Stream_Close(spSoundStream);
	result = system->createStream(sMusicFileNames[musicIndex], FMOD_LOOP_NORMAL | FMOD_2D, 0, &spSoundStream);
	FMOD_ErrorString(result);

//	spSoundStream = FSOUND_Stream_OpenFile(sMusicFileNames[musicIndex], 0, 0);
	if(spSoundStream)
	{
		//FSOUND_Stream_SetMode(spSoundStream, FSOUND_LOOP_NORMAL|FSOUND_NORMAL);
		//FSOUND_Stream_Play(MUSIC_CHANNEL, spSoundStream);
		system->playSound(spSoundStream, 0, false, &channel);
		sLastMusicIndex = musicIndex;
	}
	else
	{
		char errStr[128];
		sprintf_s(errStr, "Failed to load music file: %s", sMusicFileNames[musicIndex]);
		MessageBox(0, errStr, 0, 0);
	}
}

void FmodSound::StopMusic(void)
{
// 	FSOUND_Stream_Stop(spSoundStream);
// 	FSOUND_Stream_Close(spSoundStream);
	spSoundStream->release();
	spSoundStream = NULL;
	sLastMusicIndex = -1;
}

bool FmodSound::IsMusicStopped(void)
{
	return spSoundStream == NULL;
}
