#ifndef _CharacterMgr_H_
#define _CharacterMgr_H_

#include "../../Engine/Misc/Singleton.h"
#include <strsafe.h>

enum CharacterID
{
	CHARID_PLAYER = 0,
	CHARID_MAX
};

enum CharAttrib
{
	CAB_HP = 0,
	CAB_HPMAX = 1,
	CAB_AT = 2,
	CAB_DF = 3,
	CAB_STR = 4,
	CAB_INT = 5,
	CAB_AGL = 6,
	CAB_PHY = 7,
	CAB_LV = 8,
	CAB_EXP = 9,
	CAB_MAX
};

#define CHAR_NAME_MAX 8
class Character
{
public:
	Character(CharacterID id);
	~Character(void);

	void SetName(char name[]) { strcpy_s(mName, name); mName[4] = '\0';}
	char* GetName(void) { return mName; }
	void SetAttrib(CharAttrib index, int value) { mAttribList[index] = value; }
	int GetAttrib(CharAttrib index) { return mAttribList[index]; }
	void SetAttribList(int abList[], int count) 
	{
		assert(count == CAB_MAX);
		for(int i=0; i<CAB_MAX; i++)
		{
			mAttribList[i] = abList[i];
		}
	}
	
	void AddHP(int value) 
	{
		mAttribList[CAB_HP] += value;
		if(mAttribList[CAB_HP] > mAttribList[CAB_HPMAX])
			mAttribList[CAB_HP] = mAttribList[CAB_HPMAX];
	}
	void RemoveHP(int value) 
	{
		mAttribList[CAB_HP] -= value;
		if(mAttribList[CAB_HP] < 0)
			mAttribList[CAB_HP] = 0;
	}
	bool IsMaxHP(void) { return (mAttribList[CAB_HP] == mAttribList[CAB_HPMAX]); }

private:
	CharacterID mCharId;
	char mName[CHAR_NAME_MAX];
	int mAttribList[CAB_MAX];
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class CharacterMgr: public Singleton<CharacterMgr>
{
public:
	CharacterMgr(void);
	~CharacterMgr(void);

	Character* GetCharacter(CharacterID id) { return mpCharacters[id]; }
	
private:
	Character* mpCharacters[CHARID_MAX];
};
#define gCharacterMgr CharacterMgr::GetInstance()

#endif