#include "CharacterMgr.h"

Character::Character(CharacterID id)
:	mCharId(id)
{
	for(int i=0; i<CHAR_NAME_MAX; i++)
	{
		mName[i] = '\0';
	}

	for(int i=0; i<CAB_MAX; i++)
	{
		mAttribList[i] = 0;
	}
}
	
Character::~Character(void)
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_SINGLETON(CharacterMgr);
static CharacterMgr sCharacterMgr;

CharacterMgr::CharacterMgr(void)
{
	for(int i=0; i<CHARID_MAX; i++)
	{
		mpCharacters[i] = NULL;
	}
	mpCharacters[CHARID_PLAYER] = new Character(CHARID_PLAYER);
}

CharacterMgr::~CharacterMgr(void)
{
	for(int i=0; i<CHARID_MAX; i++)
	{
		if(mpCharacters[i])
		{
			delete mpCharacters[i];
		}
	}
}
