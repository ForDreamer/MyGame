#include "GameStatusMgr.h"

DECLARE_SINGLETON(GameStatusMgr);
static GameStatusMgr sGameStatusMgr;

GameStatusMgr::GameStatusMgr(void)
:	mState(GS_START),
	mGoldNum(0)
{
	Reset();
}
	
GameStatusMgr::~GameStatusMgr(void)
{
}

void GameStatusMgr::Reset(void)
{
	mState = GS_START;
	mGoldNum = 0;
	for(int i=0; i<ITEM_UNLOCK_MAX; i++)
		mItemsUnlocked[i] = false;
}
